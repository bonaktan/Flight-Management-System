import { Outlet } from "react-router";
import { NavLink } from "react-router";
import logo from "/app/assets/logo.png";
import { use } from "react";
import { UserContext } from "../middleware/context";

function Navbar() {
    const user = use(UserContext);
    console.log(user);
    return (
        <div className="flex justify-between h-16 items-center p-6 shadow-sm fixed top-0 w-full z-99 mb-16 bg-white">
            <NavLink to="/" className="flex items-center">
                <img src={logo} className="h-16" />
                <p>SkyBridge Airways</p>
            </NavLink>
            <div>
                {user?.logged_in ? <NavLink to="/account/dashboard">{user.user?.username}</NavLink> : <NavLink to="/auth/login">Log in</NavLink>}
            </div>
        </div>
    );
}

function Footer() {
    return (
        <div className="border-t p-6 bg-blaze-deep text-cloud-warm flex flex-col">
            <div id="top" className="flex lg:items-center justify-between border-b lg:flex-row flex-col lg:align-between items-start">
                <p className="leading-none mb-2 font-bold text-2xl">
                    SkyBridge <br />
                    Airways
                </p>
                <p>
                    &copy; 2026 SkyBridge Airways. All rights reserved. <br />
                </p>
            </div>
            <div id="bottom" className="flex flex-col mt-4">
                {[
                    { name: "Home", path: "/" },
                    { name: "About Us", path: "/about" },
                    { name: "Contact", path: "/support" },
                ].map((item) => (
                    <NavLink key={item.name} to={item.path} className="mx-2">
                        {item.name}
                    </NavLink>
                ))}
            </div>
        </div>
    );
}

export default function Layout() {
    return (
        <>
            <Navbar />
            <main className="flex-1 mt-16 relative">
                <Outlet />
            </main>
            <Footer />
        </>
    );
}
