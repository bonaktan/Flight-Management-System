// responsibiltity: fetching flight data from backend and displaying it in a list of cards

import { useEffect, useState, use } from "react";
import { NavLink, useLoaderData } from "react-router";
import axios from "axios";
import { SearchParametersContext } from "./searchContext";

export default function Search() {
    const searchParams = use(SearchParametersContext);
    // todo: passengers are not yet handled

    if (searchParams.loading) {
        return <p>Loading...</p>;
    }
    return (
        <>
            {searchParams.apiReturn ? (
                searchParams.apiReturn.map((flight) => <FlightCard key={flight.id} flight={flight} />)
            ) : searchParams.apiError ? (
                <p>Error occurred while fetching flight data. Error: {searchParams.apiError.error}</p>
            ) : (
                <p>No Flights are found.</p>
            )}
        </>
    );
}

function FlightCard({ flight }) {
    console.log(flight.id);
    const [openSidebar, setOpenSidebar] = useState(false);
    return (
        <div className="flex gap-2 w-full">
            <div className="w-2/5 p-2 border border-[#ccc] rounded-sm flex flex-col gap-2">
                <div className="flex justify-between">
                    <p>Flight {flight.id}</p>
                    <NavLink to="/booking/form">Book</NavLink>
                </div>
                <button
                    onClick={() => {
                        setOpenSidebar(!openSidebar);
                    }}>
                    Details
                </button>
                {openSidebar ? (
                    <div class="p-1 shadow-inner flex flex-wrap gap-3">
                        <p>Flight ID: {flight.id}</p>
                        <p>Departure: {flight.departure_airport_id}</p>
                        <p>Arrival: {flight.arrival_airport_id}</p>
                        <p>Base Price: {flight.base_ticket_price} PHP</p>
                        <p>Departure Time: {flight.departure}</p>
                        <p>Flight Time: {flight.flight_time}</p>
                    </div>
                ) : (
                    <></>
                )}
            </div>
            <button className="border border-[#ccc] w-1/5">P1,500</button>
            <button className="border border-[#ccc] w-1/5">P2,000</button>
            <button className="border border-[#ccc] w-1/5">P2,500</button>
        </div>
    );
}
