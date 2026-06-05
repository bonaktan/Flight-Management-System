import { NavLink } from "react-router";
import "./bookpop.css";

export default function Bookpop({ activity }) {
    return (
        <div className={`w-full border-cloud-pop border-2 p-2 flex justify-between transition ${activity ? "active" : ""} absolute left-0 bottom-0`}>
            {activity}
            <div className="flex justify-start flex-col">
                <p>Flight ID</p>
                <p className="flex items-center gap-2">
                    MNL
                    <span className="material-symbols-outlined rotate-90">flight</span>
                    CEB
                </p>
            </div>
            <div className="text-center">
                <p className="font-bold">Passengers</p>
                <p>2</p>
            </div>
            <NavLink to="/booking/form" className="border px-4">
                Book
            </NavLink>
        </div>
    );
}
