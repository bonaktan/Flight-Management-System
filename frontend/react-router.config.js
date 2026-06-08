import { authMiddleware } from "./app/middleware/auth.middleware";

export default {
    // Config options...
    // Server-side render by default, to enable SPA mode set this to `false`
    ssr: true,
    future: {
        v8_middleware: true,
    },
};
