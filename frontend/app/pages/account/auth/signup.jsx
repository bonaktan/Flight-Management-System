import { NavLink } from "react-router";

export default function Signup() {
    return (
        <div>
            <p>Accounts - Login</p>
            <NavLink to="/login">Sign Up</NavLink>
            <NavLink to="/login">Log In instead</NavLink>
        </div>
    );
}
