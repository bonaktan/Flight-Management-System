import { defineConfig } from "vite";
// import react from "@vitejs/plugin-react";
import tailwindcss from "@tailwindcss/vite";
import { reactRouter } from "@react-router/dev/vite";
// console.log( process.env.VITE_BACKEND_URL)
export default defineConfig({
    plugins: [tailwindcss(), reactRouter()],
    server: {
        proxy: {
            "/api": process.env.VITE_BACKEND_URL, // TODO: change this to an envvar
        },
    },
});
