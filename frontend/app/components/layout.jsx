import { Outlet } from "react-router";
import { NavLink } from "react-router";
function Navbar() {
    return (
        <div className="flex justify-evenly border">
            <NavLink to="/">Flight Management System</NavLink>
            <NavLink to="/login">Login (if di nakalogin)</NavLink>
            <NavLink to="/account">Account (if nakalogin)</NavLink>
        </div>
    );
}

function Footer() {
    return (
        <div className="border">
            <p>Footer</p>
            <NavLink to="/about">About</NavLink>
            <br />
            <NavLink to="/support">Support</NavLink>
        </div>
    );
}

export default function Layout() {
    return (
        <>
            <Navbar />
            <Outlet />
            <Footer />
        </>
    );
}
