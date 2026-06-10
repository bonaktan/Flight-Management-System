import { defineConfig } from "vite";
// import react from "@vitejs/plugin-react";
import tailwindcss from "@tailwindcss/vite";
import { reactRouter } from "@react-router/dev/vite";
const proxyBackend = process.env.VITE_BACKEND_URL;
console.log("Proxy defined at: ", proxyBackend);
export default defineConfig({
    plugins: [tailwindcss(), reactRouter()],
    server: {
        proxy: {
            "/api": "https://skybridge.bonnybonnybonaktan.xyz", // TODO: change this to an envvar
        },
    },
});
