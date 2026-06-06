import { use } from "react";
import "./bookpop.css";
import { SearchParametersContext } from "./searchContext";
import { useNavigate } from "react-router";

export default function Bookpop({ activity }) {
    const searchContext = use(SearchParametersContext);
    const navigate = useNavigate();
    function onConfirm() {
        navigate("/booking/form", {
            state: { flightId: searchContext.selectedFlight, class: searchContext.selectedClass, passengers: searchContext.passengers },
        });
    }
    return (
        <div className={`w-full border-cloud-pop border-2 p-2 flex justify-between transition ${activity ? "active" : ""}`}>
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
            <button className="border px-4" onClick={onConfirm}>
                Book
            </button>
        </div>
    );
}
