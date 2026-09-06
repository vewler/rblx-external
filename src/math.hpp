#pragma once
#include <cmath>
#include <algorithm>

struct Vec2 {
    float x = 0.f, y = 0.f;
    Vec2() = default;
    Vec2(float x, float y) : x(x), y(y) {}
    Vec2 operator-(const Vec2& o) const { return { x - o.x, y - o.y }; }
    Vec2 operator+(const Vec2& o) const { return { x + o.x, y + o.y }; }
    Vec2 operator*(float s) const { return { x * s, y * s }; }
    float Length() const { return sqrtf(x * x + y * y); }
    float DistTo(const Vec2& o) const { return (*this - o).Length(); }
};

struct Vec3 {
    float x = 0.f, y = 0.f, z = 0.f;
    Vec3() = default;
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}
    Vec3 operator-(const Vec3& o) const { return { x - o.x, y - o.y, z - o.z }; }
    Vec3 operator+(const Vec3& o) const { return { x + o.x, y + o.y, z + o.z }; }
    Vec3 operator*(float s) const { return { x * s, y * s, z * s }; }
    float Length() const { return sqrtf(x * x + y * y + z * z); }
    float DistTo(const Vec3& o) const { return (*this - o).Length(); }
};

struct ViewMatrix {
    float m[16]{};
    bool WorldToScreen(const Vec3& pos, Vec2& out, float screenW, float screenH) const {
        float w = pos.x * m[3] + pos.y * m[7] + pos.z * m[11] + m[15];
        if (w < 0.001f) return false;
        float invW = 1.0f / w;
        float x = pos.x * m[0] + pos.y * m[4] + pos.z * m[8]  + m[12];
        float y = pos.x * m[1] + pos.y * m[5] + pos.z * m[9]  + m[13];
        out.x = (screenW * 0.5f) * (1.0f + x * invW);
        out.y = (screenH * 0.5f) * (1.0f - y * invW);
        return (out.x >= -50.f && out.x <= screenW + 50.f && out.y >= -50.f && out.y <= screenH + 50.f);
    }
};
