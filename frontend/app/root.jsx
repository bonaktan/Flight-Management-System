import { Meta, Outlet, Scripts, ScrollRestoration, Links } from "react-router";
import "./app.css";
import { UserContext } from "./middleware/context";
import { userMiddleware } from "./middleware/auth.middleware";

export const middleware = [userMiddleware];

export async function loader({ context }) {
    const user = context.get(UserContext);
    let nonce;
    try {
        nonce = context.get("cspNonce");
    } catch {
        nonce = "";
    }
    return { cspNonce: nonce, user: user };
}

export default function Root({ loaderData }) {
    const cspNonce = loaderData.cspNonce;
    return (
        <UserContext value={loaderData.user}>
            <html lang="en">
                <head>
                    <meta charSet="utf-8" />
                    <meta name="viewport" content="width=device-width, initial-scale=1" />
                    {cspNonce && <meta property="csp-nonce" content={cspNonce} />}
                    <Meta />
                    <Links nonce={cspNonce} />
                </head>
                <body>
                    <Outlet />
                    <ScrollRestoration nonce={cspNonce} />
                    <Scripts nonce={cspNonce} />
                </body>
            </html>
        </UserContext>
    );
}
