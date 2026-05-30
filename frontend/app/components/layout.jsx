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
        <div className="border-t-1 p-6 bg-blaze-deep text-cloud-warm flex flex-col">
            <div id="top" className="flex items-center justify-between border-b-1">
                <p className="leading-none mb-2 font-bold text-2xl">
                    SkyBridge <br />
                    Airways
                </p>
                <p>
                    &copy; 2024 SkyBridge Airways. All rights reserved. <br />
                </p>
            </div>
            <div id="bottom" className="flex flex-col mt-4">
                {["Home", "About Us", "Contact", "Privacy Policy"].map((item) => (
                    <NavLink key={item} to={`/${item.toLowerCase().replace(/\s/g, "-")}`} className="mx-2">
                        {item}
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
            <main className="flex-1 mt-16">
                <Outlet />
            </main>
            <Footer />
        </>
    );
}
