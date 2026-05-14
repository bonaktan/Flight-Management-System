import { NavLink } from "react-router";

export default function Login() {
    return (
        <div>
            <p>Accounts - Login</p>
            <NavLink to="/">Log In</NavLink>
            <NavLink to="/signup">Sign Up instead</NavLink>
        </div>
    );
}
