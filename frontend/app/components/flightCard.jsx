function getDate(str, flightTime) {
    const date = new Date(str);
    const timeZone = Intl.DateTimeFormat().resolvedOptions().timeZone;
    const newDate = new Date(str);
    newDate.setSeconds(flightTime);
    console.log(newDate);
    const dateStr = date.toLocaleDateString("en-US", { year: "numeric", month: "long", day: "numeric", timeZone });
    const timeStr = date.toLocaleTimeString("en-US", { hour: "2-digit", minute: "2-digit", hour12: false, timeZone });
    const timeArrivalStr = newDate.toLocaleTimeString("en-US", { hour: "2-digit", minute: "2-digit", hour12: false, timeZone });

    return { dateStr, timeStr, timeArrivalStr };
}

export function FlightCard({ flight, isReturn }) {
    const { dateStr, timeStr, timeArrivalStr } = getDate(flight.departure, flight.flight_time);
    return (
        <div className="w-full">
            <div className="flex justify-between">
                <div>
                    <div>{isReturn ? "Return" : "Departure"}</div>
                    <div className="text-sm">{dateStr}</div>
                </div>
                <div>Economy</div>
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
            <div className="text-center">Flight Details</div>
        </div>
    );
}
