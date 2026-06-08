import { createRequestHandler } from "@react-router/express";
import express from "express";
import crypto from "crypto";
import { RouterContextProvider } from "react-router";

const app = express();
app.use(express.static("build/client"));

app.use((req, res, next) => {
    res.locals.cspNonce = crypto.randomBytes(16).toString("base64");

    res.setHeader(
        "Content-Security-Policy",
        [
            "default-src 'self'",
            `script-src 'self' 'nonce-${res.locals.cspNonce}'`,
            `style-src 'self' https://fonts.googleapis.com 'nonce-${res.locals.cspNonce}'`,
            "img-src 'self' data: https:",
            "font-src 'self' https://fonts.gstatic.com",
            "connect-src 'self'",
            "frame-ancestors 'self'",
        ].join("; "),
    );

    next();
});

app.use(
    createRequestHandler({
        build: await import("./build/server/index.js"),
        getLoadContext(req, res) {
            const context = new RouterContextProvider();
            context.set("cspNonce", res.locals.cspNonce);
            return context;
        },
    }),
);

app.listen(3000, () => {
    console.log("Frontend now running.");
});
