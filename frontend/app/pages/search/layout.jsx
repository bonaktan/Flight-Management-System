import { Outlet } from "react-router";
import { authMiddleware } from "../middleware/authMiddleware";
import { AuthContext } from "../middleware/context";
export const middleware = [authMiddleware];

export async function loader({ context }) {
    // User is guaranteed non-null here — requireAuth already checked.
    const user = context.get(AuthContext);
    return { user };
}

export default function SearchLayout({ loaderData }) {
    console.log(loaderData);
    return (
        <div>
            <p>Search View (still tba :3)</p>
            <div>
                <Outlet />
            </div>
        </div>
    );
}
