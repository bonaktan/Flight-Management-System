import { Outlet } from "react-router";
import { NavLink } from "react-router";
import logo from "/app/assets/logo.png";

function Navbar() {
    return (
        <div className="flex justify-between h-16 items-center p-6 shadow-sm fixed top-0 w-full z-99 mb-16 bg-white">
            <NavLink to="/" className="flex items-center">
                <img src={logo} className="h-16" />
                <p>SkyBridge Airways</p>
            </NavLink>
            <div>
                <NavLink to="/login">Log in</NavLink>
                {/* <NavLink to="/account">Account (if nakalogin)</NavLink>  TODO: remove once auth logic is added*/}
            </div>
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
            <main className="flex-1 mt-16">
                <Outlet />
            </main>
            <Footer />
        </>
    );
}
