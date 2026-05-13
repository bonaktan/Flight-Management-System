import { Outlet } from "react-router";

function Navbar() {
    return <p>Navbar</p>;
}

function Footer() {
    return <p>Footer</p>;
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
