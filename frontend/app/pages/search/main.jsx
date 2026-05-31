// responsibiltity: fetching flight data from backend and displaying it in a list of cards

import { useEffect, useState, use } from "react";
import { NavLink } from "react-router";
import axios from "axios";
import { SearchParametersContext } from "./searchContext";
const apiUrl = import.meta.env.VITE_BACKEND_URL;

export default function Search() {
    const [apiReturn, setApiReturn] = useState(null);
    const searchParams = use(SearchParametersContext);
    // todo: passengers are not yet handled
    useEffect(() => {
        axios
            .post(`${apiUrl}/api/search/flights`, {
                origin: searchParams.get("origin"),
                destination: searchParams.get("destination"),
                departure_date: searchParams.get("departure_date"),
                timezone: Intl.DateTimeFormat().resolvedOptions().timeZone,
                passengers: 1,
            })
            .then((ret) => {
                setApiReturn(ret.data);
                console.log(ret);
            });
    }, [searchParams]);
    return <>{apiReturn ? apiReturn.map((flight) => <FlightCard key={flight.id} flight={flight} />) : <p>No Flights are found.</p>}</>;
}

function FlightCard({ flight }) {
    console.log(flight.id);
    const [openSidebar, setOpenSidebar] = useState(false);
    return (
        <div className="border shadow-md my-2 mx-5">
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
                    <p>Frequency: {flight.frequency} (supposed to be used for calculations) </p>
                </div>
            ) : (
                <></>
            )}
        </div>
    );
}
