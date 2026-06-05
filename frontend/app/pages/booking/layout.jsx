import { NavLink, Outlet } from "react-router";
import BookingForm from "./form";

export default function BookingLayout() {
    return (
        <div>
            <div id="steps" className="flex justify-between w-full">
                <span className="material-symbols-outlined">search</span>
            </div>
            <Outlet />
            <BookingForm />
        </div>
    );
}
