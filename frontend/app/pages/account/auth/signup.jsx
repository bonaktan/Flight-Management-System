import { NavLink, Form, useNavigate, useSearchParams } from "react-router";
import { InputField } from "../../../components/input";
import { useState } from "react";
import axios from "axios";
import LoadingSprite from "../../../components/loadingSprite";

export default function Signup() {
    const [name, setName] = useState("");
    const [email, setEmail] = useState("");
    const [passwordvis, setPasswordvis] = useState(false);
    const [password, setPassword] = useState("");
    const [confirmPassword, setConfirmPassword] = useState("");
    const [error, setError] = useState("");
    const [loading, setLoading] = useState(false);
    const navigate = useNavigate();
    const searchParams = useSearchParams();
    function signup(e) {
        e.preventDefault();
        // Email validation
        const emailRegex = /^(?!\.)(?!.*\.\.)[a-z0-9_'+\-.]*[a-z0-9_+-]@([a-z0-9][a-z0-9-]*\.)+[a-z]{2,}$/;
        if (!emailRegex.test(email)) {
            setError("Invalid email address.");
            return;
        }

        // Name validation
        if (name.length < 5 || name.length > 127) {
            setError("Name must be between 5 and 127 characters.");
            return;
        }

        // Password validation
        if (password.length < 8 || password.length > 127) {
            setError("Password must be between 8 and 127 characters.");
            return;
        }

        if (password !== confirmPassword) {
            setError("Passwords do not match.");
            return;
        }
        setLoading(true);
        // TODO: sanity checking sa password, atleast 1 lowercase, uppercase, number, and special character, 8-127 chars long
        axios
            .post("/api/auth/signup", {
                name: name,
                email: email,
                password: password,
            })
            .then((e) => {
                setLoading(false);
                const returnTo = searchParams.get("returnTo");
                navigate(returnTo ? decodeURIComponent(returnTo) : "/", { reloadDocument: true }); // TODO: this shall be replaced w/ the last went link
            })
            .catch((e) => {
                setLoading(false);
                const status = e.response?.status;
                if (status === 400) {
                    setError("Inputs are misformatted.");
                } else if (status === 409) {
                    setError("An account with the same email already exists.");
                } else {
                    console.error(e);
                    setError("Something went wrong. Check Console for details.");
                }
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
                            type={passwordvis ? "text" : "password"}
                            name="Password"
                            required
                        />
                    </div>
                    <div className="my-2">
                        <InputField
                            value={confirmPassword}
                            onChange={(e) => setConfirmPassword(e.target.value)}
                            icon="key"
                            type={passwordvis ? "text" : "password"}
                            name="Confirm Password"
                            required
                        />
                    </div>
                    <div>
                        <input id="check" type="checkbox" checked={passwordvis} onChange={(e) => setPasswordvis((prev) => !prev)} />
                        <label htmlFor="check"> Show Password</label>
                    </div>
                </div>
                <button onClick={signup} className="p-3 bg-blaze-core text-cloud-warm font-bold">
                    Sign up
                </button>
                {error && <p className="text-red-500 text-center pb-4">{error}</p>}
            </Form>
            <p className="text-center pt-10">
                Already have an account?{" "}
                <span className="text-cloud-pop">
                    <NavLink to="/auth/login">Log in here</NavLink>
                </span>
                .
            </p>
            {loading && <LoadingSprite />}
        </>
    );
}
