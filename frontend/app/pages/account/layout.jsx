import { NavLink, Outlet } from "react-router";
import { authMiddleware } from "../../middleware/auth.middleware";
import axios from "axios";
import { useNavigate } from "react-router";

export const middleware = [authMiddleware];

function NavButtons({ to }) {
    return (
        <NavLink to={`/account/${to.toLowerCase()}`} className="w-1/2 bg-red-100 p-2 text-center">
            {to}
        </NavLink>
    );
}

function DetailEntry({ field, value }) {
    return (
        <div className="flex w-full">
            <div className="w-1/10 text-right">{field}</div>
            <div className="w-1/10">:</div>
            <div className="w-4/5">{value}</div>
        </div>
    );
}

function Dashboard() {
    const navigate = useNavigate();
    async function logout() {
        const user = await axios.post("/api/auth/logout");
        navigate("/auth/login");
    }
    return (
        <div className="flex flex-col gap-4">
            <div className="bg-blaze-deep flex p-2 items-center gap-2 rounded-sm">
                <div className="aspect-square h-32 bg-white rounded-full" />
                <p className="text-cloud-warm leading-tight">
                    <span className="text-3xl text-blaze-tint font-bold">Bonny</span>
                    <br />
                    Traveler
                </p>
            </div>

            <div className="flex flex-col">
                <DetailEntry field="Name" value="Timothy Magdasal" />
                <DetailEntry field="User ID" value="16" />
                <DetailEntry field="Email" value="bon@bonnybonnybonaktan.xyz" />
            </div>
            <div className="flex justify-end gap-2">
                <button onClick={logout} className="bg-red-200 p-2 rounded-sm">
                    Edit Profile
                </button>
                <button onClick={logout} className="bg-red-200 p-2 rounded-sm">
                    Log out
                </button>
            </div>
        </div>
    );
}

export default function AccountLayout() {
    return (
        <div className="flex">
            <div id="profile" className="w-1/3 p-2">
                {/* <div className="bg-blaze-tint flex p-2 items-center gap-2 rounded-sm">
                    <div className="aspect-square h-32 bg-white rounded-full" />
                        <p className="text-altitude-ink leading-tight">
                            <span className="text-3xl text-altitude-mid font-bold">Bonny</span>
                        <br/>Traveler</p>
                </div> */}
                <Dashboard />
            </div>

            {/* <div className="bg-orange-300 px-16 py-8 flex gap-8 items-center ">
                <div className="aspect-square h-32 bg-white rounded-full" />
                <div className="flex flex-col gap-2">
                    <div className="text-3xl font-bold">Bonny</div>
                    <div>Traveller</div>
                </div>
            </div> */}
            {/* <div className="flex flex-row w-full gap-2 p-2">
                <NavButtons to="Dashboard" />
                <NavButtons to="Booking" />
            </div> */}
            <div className="bg-orange-100 m-2 p-4 w-2/3">
                <Outlet />
            </div>
        </div>
    );
}
