import axios from "axios";
import { useNavigate } from "react-router";

export default function AccountSettings({ context }) {
    const navigate = useNavigate();
    async function logout() {
        const user = await axios.post("/api/auth/logout");
        navigate("/auth/login");
    }
    return (
        <div>
            <p>Account - Settings</p>
            <button onClick={logout}>Log Out</button>
        </div>
    );
}
