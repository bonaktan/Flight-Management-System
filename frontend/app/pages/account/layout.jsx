import { NavLink, Outlet } from "react-router";

import { authMiddleware } from "../../middleware/auth.middleware";
import { AuthContext } from "../../middleware/context";
export const middleware = [authMiddleware];

export function loader({ context }) {
    const user = context.get(AuthContext);
    console.log(user);
}

export default function AccountLayout() {
    return (
        <div>
            <p>Accounts - Layout</p>
            <div className="flex justify-between">
                <NavLink to="/account">Account</NavLink>
                <NavLink to="/account/booking">Booking</NavLink>
                <NavLink to="/account/settings">Settings</NavLink>
            </div>
            <Outlet />
        </div>
    );
}
