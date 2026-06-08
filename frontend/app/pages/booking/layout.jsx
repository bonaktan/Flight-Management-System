import { NavLink, Outlet } from "react-router";
import { authMiddleware } from "../../middleware/auth.middleware";

export const middleware = [authMiddleware];
export default function BookingLayout() {
    return (
        <div>
            <div id="steps" className="flex justify-between w-full">
                <span className="material-symbols-outlined">search</span>
            </div>
            <Outlet />
        </div>
    );
}
