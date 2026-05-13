import { defineConfig } from "vite";
// import react from "@vitejs/plugin-react";
import tailwindcss from "@tailwindcss/vite";
import { reactRouter } from "@react-router/dev/vite";

export default defineConfig({
    plugins: [tailwindcss(), reactRouter()],
});
