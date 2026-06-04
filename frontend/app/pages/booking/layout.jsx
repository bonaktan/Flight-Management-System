import { NavLink, Outlet } from "react-router";
import { authMiddleware } from "../../middleware/auth.middleware";

export const middleware = [authMiddleware];
export default function BookingLayout() {
    return (
        <div>
            <p>Booking - Layout</p>
            <div className="flex justify-between">
                <NavLink to="/booking/form">Step 1</NavLink>
                <NavLink to="/booking/details">Step 2</NavLink>
                <NavLink to="/booking/payment">Step 3</NavLink>
                <NavLink to="/booking/confirmation">Step 4</NavLink>
            </div>
            <Outlet />
        </div>
    );
}
