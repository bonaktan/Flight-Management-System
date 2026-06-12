import express from "express";
import path from "path";
import { createProxyMiddleware } from "http-proxy-middleware";
const app = express();

app.use(express.static("build/client"));

const backendUrl = process.env.VITE_BACKEND_URL || "http://localhost:8080";

// Proxy /api before static files
app.use(
    "/api",
    createProxyMiddleware({
        target: backendUrl,
        changeOrigin: true,
        pathRewrite: { "^/api": "/api" },
    }),
);

app.use((req, res, next) => {
    res.setHeader(
        "Content-Security-Policy",
        [
            "default-src 'self'",
            "script-src 'self'",
            "style-src 'self' https://fonts.googleapis.com",
            "img-src 'self' data: https:",
            "font-src 'self' https://fonts.gstatic.com",
            "connect-src 'self'",
            "frame-ancestors 'self'",
        ].join("; "),
    );
    next();
});

// SPA fallback
app.get("*", (req, res) => {
    res.sendFile(path.resolve("build/client/index.html"));
});

app.listen(3000, () => {
    console.log("Frontend now running.");
});
