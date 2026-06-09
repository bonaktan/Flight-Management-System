import { NavLink } from "react-router";
import { FlightCard } from "../../components/flightCard";
import { OverlayBase, OverlaySidebar } from "../../components/overlay";
import { useState } from "react";

function FlightDetailSidebar({ flight }) {
    return (
        <OverlaySidebar className="right-0 p-6">
            <div className="flex flex-col gap-10">
                <div>
                    <div className="text-2xl font-semibold">Flight Details</div>
                    <FlightCard flight={flight.departureFlight} />
                    {flight.isRoundTrip && <FlightCard flight={flight.returnFlight} isReturn={true} />}
                </div>
                <div>
                    <div className="text-xl font-semibold pb-2 ">Passenger Details</div>
                    <div className="flex flex-col gap-2">
                        {flight.passengers.map((passenger) => (
                            <PassengerDetail key={passenger.seat} passenger={passenger} />
                        ))}
                    </div>
                </div>
            </div>
        </OverlaySidebar>
    );
}

function PassengerDetail({ passenger }) {
    return (
        <div className="flex w-full bg-blaze-tint p-2 items-center rounded-lg">
            <div className="w-4/5 px-4">
                {passenger.name} - Seat {passenger.seat}
            </div>
            <div className="w-1/5">
                <button className="py-2 px-4 bg-blaze-core text-cloud-warm font-semibold rounded-md">Print</button>
            </div>
        </div>
    );
}

function BookingCard({ flight }) {
    const [openSidebar, setOpenSidebar] = useState(false);
    return (
        <div className="flex justify-center items-center w-full ">
            <div className="bg-white px-2 w-4/5 ">
                <div className="flex gap-4">
                    <div className="w-4/5 p-4">
                        <p>{flight.passenger_name}</p>
                        <div className="flex gap-10">
                            <FlightCard flight={flight.departureFlight} />
                            {flight.isRoundTrip && <FlightCard flight={flight.returnFlight} isReturn={true} />}
                        </div>
                    </div>
                    <button onClick={() => setOpenSidebar(true)} className="flex items-center justify-center w-1/5 border-l">
                        Details
                    </button>
                </div>
            </div>
            <OverlayBase open={openSidebar} setOpen={setOpenSidebar}>
                <FlightDetailSidebar flight={flight} />
            </OverlayBase>
        </div>
    );
}

export function loader() {
    const mockApi = [
        {
            isRoundTrip: false,
            departureFlight: { flightId: "SKY067", departure: "2026-06-30T15:00:00+08:00", flight_time: 5400, origin: "MNL", destination: "CEB" },
            passengers: [
                {
                    name: "Ms. Bonny Bon",
                    seat: "A1",
                },
            ],
        },
        {
            isRoundTrip: true,
            departureFlight: { flightId: "SKY067", departure: "2026-06-30T15:00:00+08:00", flight_time: 5400, origin: "MNL", destination: "CEB" },
            returnFlight: { flightId: "SKY069", departure: "2026-07-30T15:00:00+08:00", flight_time: 5400, origin: "CEB", destination: "MNL" },
            passengers: [
                {
                    name: "Ms. Bonny Bon",
                    seat: "A1",
                },
            ],
        },
        {
            isRoundTrip: false,
            departureFlight: { flightId: "SKY067", departure: "2026-06-30T15:00:00+08:00", flight_time: 5400, origin: "MNL", destination: "CEB" },
            passengers: [
                {
                    name: "Ms. Bonny Bon",
                    seat: "A1",
                },
                {
                    name: "Ms. Bonnyyyy Bon",
                    seat: "A2",
                },
            ],
        },
    ];
    return { bookings: mockApi };
}
export default function AccountBooking({ loaderData }) {
    return (
        <div className="flex flex-col gap-2">
            <div className="text-2xl font-semibold text-center">Bookings </div>
            {loaderData.bookings.map((flight, key) => (
                <BookingCard key={key} flight={flight} />
            ))}
        </div>
    );
}
