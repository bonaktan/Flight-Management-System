import { NavLink } from "react-router";
import { FlightCard } from "../../../components/flightCard";

function BookingCard({ flight }) {
    return (
        <div className="flex justify-center items-center">
            <div className="bg-white p-2 w-4/5 ">
                <p>{flight.passenger_name}</p>
                <div className="flex gap-10">
                    <FlightCard flight={flight.departureFlight} />
                    {flight.isRoundTrip && <FlightCard flight={flight.returnFlight} isReturn={true} />}
                </div>
            </div>
            {/* <NavLink to="/account/booking/details" className="text-center w-1/5">
                Details
            </NavLink> */}
        </div>
    );
}
export function loader() {
    const mockApi = [
        {
            isRoundTrip: true,
            departureFlight: { flightId: "SKY067", departure: "2026-06-30T15:00:00+08:00", flight_time: 5400, origin: "MNL", destination: "CEB" },
            returnFlight: { flightId: "SKY069", departure: "2026-07-30T15:00:00+08:00", flight_time: 5400, origin: "CEB", destination: "MNL" },
            passenger_name: "Ms. Bonny Bon",
        },
        {
            isRoundTrip: true,
            departureFlight: { flightId: "SKY067", departure: "2026-06-30T15:00:00+08:00", flight_time: 5400, origin: "MNL", destination: "CEB" },
            returnFlight: { flightId: "SKY069", departure: "2026-07-30T15:00:00+08:00", flight_time: 5400, origin: "CEB", destination: "MNL" },
            passenger_name: "Ms. Bonny Bon",
        },
        {
            isRoundTrip: false,
            departureFlight: { flightId: "SKY067", departure: "2026-06-30T15:00:00+08:00", flight_time: 5400, origin: "MNL", destination: "CEB" },
            passenger_name: "Ms. Bonny Bon",
        },
    ];
    return { bookings: mockApi };
}
export default function AccountBooking({ loaderData }) {
    return (
        <div className="flex flex-col gap-2">
            {loaderData.bookings.map((flight, key) => (
                <BookingCard key={key} flight={flight} />
            ))}
        </div>
    );
}
