import { NavLink, Outlet } from "react-router";
import { authMiddleware } from "../../middleware/auth.middleware";

export const middleware = [authMiddleware];

function NavButtons({ to }) {
    return (
        <NavLink to={`/account/${to.toLowerCase()}`} className="w-1/2 bg-red-100 p-2 text-center">
            {to}
        </NavLink>
    );
}
export default function AccountLayout() {
    return (
        <div>
            <div className="bg-orange-300 px-16 py-8 flex gap-8 items-center ">
                <div className="aspect-square h-32 bg-white rounded-full" />
                <div className="flex flex-col gap-2">
                    <div className="text-3xl font-bold">Bonny</div>
                    <div>Traveller</div>
                </div>
            </div>
            <div className="flex flex-row w-full gap-2 p-2">
                <NavButtons to="Dashboard" />
                <NavButtons to="Booking" />
            </div>
            <div className="bg-orange-100 m-2 p-4">
                <Outlet />
            </div>
        </div>
    );
}
