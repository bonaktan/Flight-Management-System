import axios from "axios";
import { useNavigate } from "react-router";

function DetailEntry({ field, value }) {
    return (
        <div className="flex w-full">
            <div className="w-1/10 text-right">{field}</div>
            <div className="w-1/10">:</div>
            <div className="w-4/5">{value}</div>
        </div>
    );
}

export default function Dashboard() {
    const navigate = useNavigate();
    async function logout() {
        const user = await axios.post("/api/auth/logout");
        navigate("/auth/login");
    }
    return (
        <div>
            <div className="text-2xl font-bold">Dashboard</div>
            <div className="flex flex-col">
                <DetailEntry field="Name" value="Timothy Magdasal" />
                <DetailEntry field="User ID" value="16" />
                <DetailEntry field="Email" value="bon@bonnybonnybonaktan.xyz" />
            </div>
            <div className="flex justify-end">
                <button onClick={logout} className="bg-red-200 p-2 mr-4">
                    Log out
                </button>
            </div>
        </div>
    );
}
