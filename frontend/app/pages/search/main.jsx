import { useEffect, useState } from "react";
import { NavLink } from "react-router";
import axios from "axios";
const apiUrl = "http://localhost:8080";
export default function Search() {
    const [apiReturn, setApiReturn] = useState([]);
    useEffect(() => {
        axios.get(`${apiUrl}/api/search/flights`).then((ret) => {
            setApiReturn(ret.data);
            console.log(ret);
        });
    }, []);
    return (
        <>
            {apiReturn.map((flight) => (
                <FlightCard key={flight.id} flight={flight} />
            ))}
        </>
    );
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
