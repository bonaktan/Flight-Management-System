import { NavLink, Form } from "react-router";
import InputField from "../../../components/input";

export default function Login() {
    return (
        <>
            <p className="text-2xl text-center p-4">Sign Up</p>
            <Form className="flex flex-col">
                <div className="my-5">
                    <div className="my-2">
                        <InputField icon="mail" type="email" name="Email" required />
                    </div>
                    <div className="my-2">
                        <InputField icon="person" type="text" name="Name" required />
                    </div>
                    <div className="my-2">
                        <InputField icon="key" type="email" name="Password" required />
                    </div>
                    <div className="my-2">
                        <InputField icon="key" type="password" name="Confirm Password" required />
                    </div>
                </div>
                <button type="submit" className="p-3 bg-red-400">
                    Sign up
                </button>
            </Form>
            <p className="text-center pt-10">
                Already have an account? <NavLink to="/auth/login">Log in here</NavLink>.
            </p>
        </>
    );
}
