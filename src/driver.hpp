#pragma once
#include <Windows.h>
#include <winioctl.h>
#include <cstdint>
#include <string>
#include <vector>

#define IOCTL_READ_MEMORY  CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IOCTL_WRITE_MEMORY CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IOCTL_GET_BASE     CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

struct KernelReadRequest {
    ULONG pid;
    ULONG64 address;
    ULONG64 buffer;
    ULONG size;
};

struct KernelWriteRequest {
    ULONG pid;
    ULONG64 address;
    ULONG64 buffer;
    ULONG size;
};

struct KernelBaseRequest {
    ULONG pid;
    ULONG64 baseAddress;
};

class Driver {
private:
    HANDLE hDevice = INVALID_HANDLE_VALUE;
    DWORD targetPid = 0;
    uintptr_t baseAddress = 0;

public:
    bool Initialize(const char* deviceName = "\\\\.\\NullKD") {
        hDevice = CreateFileA(
            deviceName,
            GENERIC_READ | GENERIC_WRITE,
            FILE_SHARE_READ | FILE_SHARE_WRITE,
            nullptr, OPEN_EXISTING, 0, nullptr
        );
        return hDevice != INVALID_HANDLE_VALUE;
    }

    void Shutdown() {
        if (hDevice != INVALID_HANDLE_VALUE) {
            CloseHandle(hDevice);
            hDevice = INVALID_HANDLE_VALUE;
        }
    }

    bool AttachToProcess(DWORD pid) {
        targetPid = pid;
        KernelBaseRequest req{};
        req.pid = pid;
        DWORD bytes = 0;
        bool ok = DeviceIoControl(hDevice, IOCTL_GET_BASE, &req, sizeof(req), &req, sizeof(req), &bytes, nullptr);
        if (ok && req.baseAddress) {
            baseAddress = req.baseAddress;
            return true;
        }
        return false;
    }

    uintptr_t GetBaseAddress() const { return baseAddress; }
    DWORD GetPid() const { return targetPid; }

    bool ReadRaw(uintptr_t address, void* buffer, size_t size) const {
        if (hDevice == INVALID_HANDLE_VALUE || !targetPid) return false;
        KernelReadRequest req{};
        req.pid = targetPid;
        req.address = address;
        req.buffer = reinterpret_cast<ULONG64>(buffer);
        req.size = static_cast<ULONG>(size);
        DWORD bytes = 0;
        return DeviceIoControl(hDevice, IOCTL_READ_MEMORY, &req, sizeof(req), &req, sizeof(req), &bytes, nullptr);
    }

    bool WriteRaw(uintptr_t address, const void* buffer, size_t size) const {
        if (hDevice == INVALID_HANDLE_VALUE || !targetPid) return false;
        KernelWriteRequest req{};
        req.pid = targetPid;
        req.address = address;
        req.buffer = reinterpret_cast<ULONG64>(buffer);
        req.size = static_cast<ULONG>(size);
        DWORD bytes = 0;
        return DeviceIoControl(hDevice, IOCTL_WRITE_MEMORY, &req, sizeof(req), &req, sizeof(req), &bytes, nullptr);
    }

    template<typename T>
    T Read(uintptr_t address) const {
        T val{};
        ReadRaw(address, &val, sizeof(T));
        return val;
    }

    template<typename T>
    bool Write(uintptr_t address, const T& value) const {
        return WriteRaw(address, &value, sizeof(T));
    }

    std::string ReadRobloxString(uintptr_t ptrAddr) const {
        uintptr_t strObj = Read<uintptr_t>(ptrAddr);
        if (!strObj) return "";
        int len = Read<int>(strObj + 0x10);
        if (len <= 0 || len > 256) return "";
        uintptr_t dataPtr = (len < 16) ? strObj : Read<uintptr_t>(strObj);
        if (!dataPtr) return "";
        std::vector<char> buf(len + 1, 0);
        ReadRaw(dataPtr, buf.data(), len);
        return std::string(buf.data(), len);
    }
};

inline Driver g_driver;
