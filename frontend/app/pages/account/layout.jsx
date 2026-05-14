import { NavLink, Outlet } from "react-router";

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
