// responsibiltity: fetching flight data from backend and displaying it in a list of cards

import { useEffect, useState, use } from "react";
import { NavLink, useLoaderData } from "react-router";
import axios from "axios";
import { SearchParametersContext } from "./searchContext";
import { addTime } from "../../components/datetime";
export default function Search() {
    const searchParams = use(SearchParametersContext);
    // todo: passengers are not yet handled
    console.log(searchParams);
    if (!searchParams) {
        return <p>Loading...</p>;
    }
    return (
        <div>
            {searchParams.apiReturn ? (
                searchParams.apiReturn.map((flight) => <FlightCard key={flight.id} flight={flight} />)
            ) : searchParams.apiError ? (
                <p>Error occurred while fetching flight data. Error: {searchParams.apiError.error}</p>
            ) : (
                <p>No Flights are found.</p>
            )}
        </div>
    );
}

function FlightCard({ flight }) {
    console.log(flight.id);
    const [selectedClass, setSelectedClass] = useState();
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
            {[flight.base_ticket_price, flight.base_ticket_price * 2, flight.base_ticket_price * 4].map((price) => (
                <button
                    onClick={() => {
                        setSelectedClass(price);
                        console.log(price);
                    }}
                    key={price}
                    className={`transition border boorder-[#ccc] w-1/5 ${selectedClass == price ? "bg-blaze-core" : ""}`}>
                    {price}
                </button>
            ))}

            {/* <button className="border border-[#ccc] w-1/5">P1,500</button>
            <button className="border border-[#ccc] w-1/5">P2,000</button>
            <button className="border border-[#ccc] w-1/5">P2,500</button> */}
        </div>
    );
}
