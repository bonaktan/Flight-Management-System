import axios from "axios";
import { useEffect, useState } from "react";

function getDate(str, flightTime) {
    const date = new Date(str);
    const timeZone = Intl.DateTimeFormat().resolvedOptions().timeZone;
    const newDate = new Date(date.getTime() + flightTime * 1000);
    const dateStr = date.toLocaleDateString("en-US", { year: "numeric", month: "long", day: "numeric", timeZone });
    const timeStr = date.toLocaleTimeString("en-US", { hour: "2-digit", minute: "2-digit", hour12: false, timeZone });
    const timeArrivalStr = newDate.toLocaleTimeString("en-US", { hour: "2-digit", minute: "2-digit", hour12: false, timeZone });

    return { dateStr, timeStr, timeArrivalStr };
}

export function FlightCard({ oldflight, isReturn, flightId }) {
    const [flight, setFlight] = useState(oldflight);
    console.log(flightId);
    useEffect(() => {
        console.log(flightId);
        if (flightId == null) return;

        axios.get(`/api/search/flight/${flightId}`).then((res) => setFlight(res.data));
    }, [flightId]);
    if (!flight) return <></>;
    // console.
    const { dateStr, timeStr, timeArrivalStr } = getDate(flight.departure, flight.flight_time);
    return (
        <div className="w-full">
            <div className="flex justify-between">
                <div>
                    <div className="text-lg font-semibold">{flight.flightId}</div>

                    <div className="text-sm">{dateStr}</div>
                </div>
                <div>{isReturn ? "Return" : "Departure"}</div>
            </div>
            <div className="flex justify-between items-center mt-2">
                <div>
                    <div>{timeStr}</div>
                    <div>{flight.origin}</div>
                </div>
                <span className="material-symbols-outlined">{isReturn ? "flight_land" : "flight_takeoff"}</span>
                <div>
                    <div>{timeArrivalStr}</div>
                    <div>{flight.destination}</div>
                </div>
            </div>
        </div>
    );
}
