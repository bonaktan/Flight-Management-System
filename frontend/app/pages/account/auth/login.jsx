import { NavLink, useNavigate } from "react-router";
import { InputField } from "../../../components/input";
import { useState } from "react";
import axios from "axios";

export default function Login() {
    const [username, setUsername] = useState("");
    const [passwordvis, setPasswordvis] = useState(false);
    const [password, setPassword] = useState("");
    const [error, setError] = useState("");
    const navigate = useNavigate();
    function login(e) {
        e.preventDefault();
        axios
            .post("/api/auth/login", {
                email: username,
                password: password,
            })
            .then((e) => {
                navigate("/"); // TODO: this shall be replaced w/ the last went link
            })
            .catch((e) => {
                if (e.status == 401) {
                    setError("Invalid Credentials.");
                } else {
                    console.error(e);
                    setError("Something went wrong. Check Console for details.");
                }
            });
    }

    return (
        <>
            <p className="text-2xl text-center p-4">Login</p>
            <div className="flex flex-col">
                <div className="my-5">
                    <div className="my-2">
                        <InputField value={username} onChange={(e) => setUsername(e.target.value)} icon="mail" type="email" name="Email" required />
                    </div>
                    <div className="my-2">
                        <InputField
                            value={password}
                            onChange={(e) => setPassword(e.target.value)}
                            icon="key"
                            type={passwordvis ? "text" : "password"}
                            name="Password"
                            required
                        />
                    </div>
                    <div>
                        <input id="check" type="checkbox" checked={passwordvis} onChange={(e) => setPasswordvis((prev) => !prev)} />
                        <label htmlFor="check"> Show Password</label>
                    </div>
                </div>
                <button onClick={login} className="p-3 bg-blaze-core text-cloud-warm font-bold">
                    Login
                </button>
            </div>
            <p className="text-center pt-10">
                Don't have an account?{" "}
                <span className="text-cloud-pop">
                    <NavLink to="/auth/signup">Sign up here</NavLink>
                </span>
                .
            </p>
            <p className="text-center">{error}</p>
        </>
    );
}
