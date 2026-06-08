import { defineConfig } from "vite";
// import react from "@vitejs/plugin-react";
import tailwindcss from "@tailwindcss/vite";
import { reactRouter } from "@react-router/dev/vite";

export default defineConfig({
    plugins: [tailwindcss(), reactRouter()],
    server: {
        proxy: {
            "/api": "http://localhost:8080", // TODO: change this to an envvar
        },
    },
});
