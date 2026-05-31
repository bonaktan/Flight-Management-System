import { Meta, Outlet, Scripts, ScrollRestoration, Links } from "react-router";
import "./app.css";

export async function loader({ context }) {
    return { cspNonce: context.cspNonce };
}

export default function Root({ loaderData }) {
    const { cspNonce } = loaderData;
    return (
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
    );
}
