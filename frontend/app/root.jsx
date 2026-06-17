import { Meta, Outlet, Scripts, ScrollRestoration, Links } from "react-router";
import LoadingSprite from "./components/loadingSprite";
import "./app.css";
import { UserContext } from "./middleware/context";
import { userMiddleware } from "./middleware/auth.middleware";
import { useNavigation } from "react-router";

export const clientMiddleware = [userMiddleware];

export async function clientLoader({ context }) {
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
    const navigation = useNavigation();
    const isLoading = navigation.state !== "idle";
    const cspNonce = loaderData.cspNonce;
    return (
        <UserContext value={loaderData.user}>
            <html lang="en">
                <head>
                    <meta charSet="utf-8" />
                    <meta name="viewport" content="width=device-width, initial-scale=1" />
                    {cspNonce && <meta property="csp-nonce" />}
                    <Meta />
                    <Links />
                </head>
                <body>
                    {isLoading && <LoadingSprite />}
                    <Outlet />
                    <ScrollRestoration />
                    <Scripts />
                </body>
            </html>
        </UserContext>
    );
}
