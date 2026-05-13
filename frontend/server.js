import { createRequestHandler } from "@react-router/express";
import express from "express";

const app = express();

app.use(express.static("build/client"));
app.use(
    createRequestHandler({
        build: await import("./build/server/index.js"),
    }),
);

app.listen(3000, () => {
    console.log(`Server is running on http://localhost:3000`);
});
