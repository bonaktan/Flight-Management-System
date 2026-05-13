import { NavLink } from "react-router";

export default function Home() {
    return (
        <div>
            <p>Home</p>
            <NavLink to="/search">Search for Flights</NavLink>
        </div>
    );
}
