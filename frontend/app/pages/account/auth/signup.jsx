import { NavLink, Form, useNavigate } from "react-router";
import InputField from "../../../components/input";
import { useState } from "react";
import axios from "axios";

export default function Signup() {
    const [name, setName] = useState("");
    const [email, setEmail] = useState("");
    const [password, setPassword] = useState("");
    const [confirmPassword, setConfirmPassword] = useState("");
    const [error, setError] = useState("");
    const navigate = useNavigate();
    function signup(e) {
        e.preventDefault();
        if (password != confirmPassword) {
            setError("Passwords does not match.");
        }
        // TODO: sanity checking sa password, atleast 1 lowercase, uppercase, number, and special character, 8-127 chars long
        axios
            .post("/api/auth/signup", {
                name: name,
                email: email,
                password: password,
            })
            .then((e) => {
                console.log(e);
                navigate("/"); // TODO: this shall be replaced w/ the last went link
            })
            .catch((e) => {
                console.error(e);
            });
    }
    return (
        <>
            <p className="text-2xl text-center p-4">Sign Up</p>
            <Form className="flex flex-col">
                <div className="my-5">
                    <div className="my-2">
                        <InputField value={email} onChange={(e) => setEmail(e.target.value)} icon="mail" type="email" name="Email" required />
                    </div>
                    <div className="my-2">
                        <InputField value={name} onChange={(e) => setName(e.target.value)} icon="person" type="text" name="Name" required />
                    </div>
                    <div className="my-2">
                        <InputField
                            value={password}
                            onChange={(e) => setPassword(e.target.value)}
                            icon="key"
                            type="password"
                            name="Password"
                            required
                        />
                    </div>
                    <div className="my-2">
                        <InputField
                            value={confirmPassword}
                            onChange={(e) => setConfirmPassword(e.target.value)}
                            icon="key"
                            type="password"
                            name="Confirm Password"
                            required
                        />
                    </div>
                </div>
                <button onClick={signup} className="p-3 bg-blaze-core text-cloud-warm font-bold">
                    Sign up
                </button>
            </Form>
            <p className="text-center pt-10">
                Already have an account?{" "}
                <span className="text-cloud-pop">
                    <NavLink to="/auth/login">Log in here</NavLink>
                </span>
                .
            </p>
        </>
    );
}
