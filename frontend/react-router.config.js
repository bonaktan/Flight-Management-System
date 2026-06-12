import { authMiddleware } from "./app/middleware/auth.middleware";

export default {
    // Config options...
    // Server-side render by default, to enable SPA mode set this to `false`
    ssr: false,
    future: {
        v8_middleware: true,
        unstable_middleware: true,
    },
};
