import { Meta, Outlet, Scripts, ScrollRestoration, Links } from "react-router";
import "./app.css";
import { UserContext } from "./middleware/context";
import { userMiddleware } from "./middleware/auth.middleware";

export const middleware = [userMiddleware];

export async function loader({ context }) {
    const user = context.get(UserContext);
    return { cspNonce: context.cspNonce, user: user };
}

export default function Root({ loaderData }) {
    const { cspNonce } = loaderData;
    return (
        <UserContext value={loaderData.user}>
            <html lang="en">
                <head>
                    <meta charSet="utf-8" />
                    <meta name="viewport" content="width=device-width, initial-scale=1" />
                    <Meta />
                    <Links />
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
