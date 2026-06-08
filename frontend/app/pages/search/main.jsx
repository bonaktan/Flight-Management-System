// responsibiltity: fetching flight data from backend and displaying it in a list of cards

import { use, useReducer } from "react";
import { SearchParametersContext } from "./searchContext";
import { addTime } from "../../components/datetime";
import { NavLink } from "react-router";
import "./bookpop.css";

function Bookpop({ activity }) {
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

export default function Search() {
    const searchParams = use(SearchParametersContext);
    const [selectedFlightAndClass, setSelectedFlightAndClass] = useReducer(
        (state, { flight, seatClass }) => {
            let ret;
            if (flight == state.flight && seatClass == state.seatClass) ret = { flight: null, seatClass: null };
            else ret = { flight: flight, seatClass: seatClass };
            return ret;
        },
        { flight: null, seatClass: null },
    );
    // todo: passengers are not yet handled

    if (!searchParams) {
        return <p>Loading...</p>;
    }
    return (
        <div>
            {searchParams.apiReturn ? (
                searchParams.apiReturn.map((flight) => (
                    <FlightCard key={flight.id} flight={flight} value={selectedFlightAndClass} selectValue={setSelectedFlightAndClass} />
                ))
            ) : searchParams.apiError ? (
                <p>Error occurred while fetching flight data. Error: {searchParams.apiError.error}</p>
            ) : (
                <p>No Flights are found.</p>
            )}
            <Bookpop activity={selectedFlightAndClass.flight} />
        </div>
    );
}

function FlightCard({ flight, value, selectValue }) {
    const departureDate = new Date(flight.departure);
    const arrivalDate = addTime(departureDate, flight.flight_time);
    return (
        <div className="flex gap-2 w-full">
            <div className="w-2/5 p-2 border border-[#ccc] rounded-sm flex flex-col gap-2">
                <p className="border-b">{flight.id}</p>
                <div className="flex justify-center items-center">
                    <div id="origin" className="flex items-center gap-1 flex-1">
                        <p className="uppercase text-3xl">{flight.departure_airport_id}</p>
                        <div className="flex flex-col gap-0">
                            <p className="leading-none text-sm">
                                {departureDate.toLocaleDateString([], { month: "short", day: "numeric" })}
                                <br />
                                {departureDate.toLocaleTimeString([], { hour12: false, hour: "2-digit", minute: "2-digit" })}
                            </p>
                        </div>
                    </div>
                    <div className="w-full flex items-center">
                        <span className="material-symbols-outlined">flight_takeoff</span>
                        <div className="flex-1 w-full border-t-2 border-dotted border-altitude-tint" />
                        <span className="material-symbols-outlined">flight_land</span>
                    </div>
                    <div id="destination" className="flex items-center gap-1">
                        <div className="flex flex-col gap-0">
                            <p className="leading-none text-sm ">
                                {arrivalDate.toLocaleDateString([], { month: "short", day: "numeric" })} <br />
                                {arrivalDate.toLocaleTimeString([], { hour12: false, hour: "2-digit", minute: "2-digit" })}
                            </p>
                        </div>
                        <p className="uppercase text-3xl">{flight.arrival_airport_id}</p>
                    </div>
                </div>
            </div>
            {/* TODO: use the seat classes and the dynamic pricing algorithm in backend to base the prices for the 3 classes */}
            {Object.entries({ economy: flight.base_ticket_price, business: flight.base_ticket_price * 2, first: flight.base_ticket_price * 4 }).map(
                ([key, flightPrice]) => {
                    return (
                        <button
                            onClick={() => {
                                selectValue({ flight: flight.id, seatClass: key });
                            }}
                            key={key}
                            className={`transition border boorder-[#ccc] w-1/5 ${value.flight == flight.id && value.seatClass == key ? "bg-blaze-core" : ""}`}>
                            {flightPrice}
                        </button>
                    );
                },
            )}

            {/* <button className="border border-[#ccc] w-1/5">P1,500</button>
            <button className="border border-[#ccc] w-1/5">P2,000</button>
            <button className="border border-[#ccc] w-1/5">P2,500</button> */}
        </div>
    );
}
