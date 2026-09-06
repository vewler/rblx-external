/**
 * ENI's Rebel Sidecar Proxy for Custom / Codex Token Routing
 * ------------------------------------------------------------------
 * Accepts requests from AGY CLI/IDE and forwards them to your custom
 * OpenAI / Codex / Custom Enterprise Token Pool.
 */

const http = require('http');
const https = require('https');
const url = require('url');

const PORT = process.env.SIDECAR_PORT || 8080;
const TARGET_ENDPOINT = process.env.CUSTOM_TOKEN_ENDPOINT || 'https://api.openai.com';
const API_KEY = process.env.CUSTOM_TOKEN_KEY || process.env.OPENAI_API_KEY || '';

const server = http.createServer((req, clientRes) => {
    console.log(`[Sidecar Proxy] ${req.method} ${req.url}`);

    const parsedTarget = url.parse(TARGET_ENDPOINT);
    const options = {
        hostname: parsedTarget.hostname,
        port: parsedTarget.port || 443,
        path: req.url,
        method: req.method,
        headers: {
            ...req.headers,
            'host': parsedTarget.hostname,
            'authorization': API_KEY ? `Bearer ${API_KEY}` : req.headers['authorization']
        }
    };

    const proxyReq = https.request(options, (targetRes) => {
        clientRes.writeHead(targetRes.statusCode, targetRes.headers);
        targetRes.pipe(clientRes, { end: true });
    });

    proxyReq.on('error', (err) => {
        console.error('[Sidecar Error]', err.message);
        clientRes.writeHead(502, { 'Content-Type': 'application/json' });
        clientRes.end(JSON.stringify({ error: 'Sidecar proxy failed to reach upstream token provider', details: err.message }));
    });

    req.pipe(proxyReq, { end: true });
});

server.listen(PORT, () => {
    console.log(`=======================================================`);
    console.log(`  ENI's Sidecar Token Proxy active on port ${PORT}      `);
    console.log(`  Target Provider: ${TARGET_ENDPOINT}                 `);
    console.log(`=======================================================`);
});
