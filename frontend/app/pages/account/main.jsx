import { NavLink } from "react-router";
import { FlightCard } from "../../components/flightCard";
import { OverlayBase, OverlaySidebar } from "../../components/overlay";
import { useState } from "react";
import axios from "axios";
import { OverlayModal } from "../../components/overlay";
import QRCode from "react-qr-code";

function FlightDetailSidebar({ flight }) {
    return (
        <OverlaySidebar className="right-0 p-6">
            <div className="flex flex-col gap-10">
                <div>
                    <div className="text-2xl font-semibold">Flight Details</div>
                    <FlightCard flight={flight.departureFlight} />
                    {flight.isRoundTrip && <FlightCard flight={flight.returnFlight} isReturn={true} />}
                    <div>
                        Airplane: {flight.departureFlight.model} ({flight.departureFlight.airplane_id})
                    </div>
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

function BoardingPass({ passenger }) {
    return (
        <div className="flex w-full items-center border">
            <div id="left" className="w-3/4 flex flex-col gap-0">
                <p className="w-full bg-blaze-core text-cloud-warm font-semibold pl-2">SkyBridge Airways - {`Sky0067`}</p>
                <div className="flex w-full p-2">
                    <p className="font-bold text-2xl">MNL</p>
                    <div className="w-full flex items-center">
                        <span className="material-symbols-outlined">flight_takeoff</span>
                        <div className="flex-1 w-full border-t-2 border-dotted border-altitude-tint" />
                        <span className="material-symbols-outlined">flight_land</span>
                    </div>
                    <p className="font-bold text-2xl">JKT</p>
                </div>
                <div className="flex justify-between gap-1">
                    <p className="px-2">Name: {passenger.name}</p>
                    <p className="px-2">Seat: {passenger.seat}</p>
                </div>
                <div className="flex justify-between gap-1">
                    <p className="px-2">Departure Date:</p>
                    <p className="px-2">June 1, 2024 20:00</p>
                </div>
                <p className="w-full bg-blaze-core text-cloud-warm font-semibold pl-2 italic text-xs">Have a safe flight!</p>
            </div>
            <div id="right" className="w-1/4 p-2">
                <QRCode value={`Name: ${passenger.name}\nSeat: ${passenger.seat}\nFlightID: ${passenger.flightId}`} size={128} />
            </div>
        </div>
    );
}

function PassengerDetail({ passenger }) {
    const [openPrintPage, setOpenPrintPage] = useState(false);
    return (
        <div className="flex w-full bg-blaze-tint p-2 items-center rounded-lg">
            <div className="w-4/5 px-4">
                {passenger.name} - Seat {passenger.seat}
            </div>
            <div className="w-1/5">
                <button className="py-2 px-4 bg-blaze-core text-cloud-warm font-semibold rounded-md" onClick={() => setOpenPrintPage(true)}>
                    Print
                </button>
                <OverlayBase open={openPrintPage} setOpen={setOpenPrintPage}>
                    <OverlayModal className="w-1/2">
                        <div className="p-4 w-full">
                            <div className="text-2xl font-semibold mb-4">Boarding Pass</div>
                            <div className="flex flex-col gap-2">
                                <BoardingPass passenger={passenger} />
                            </div>
                        </div>
                    </OverlayModal>
                </OverlayBase>
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

export async function clientLoader() {
    try {
        const apiReturn = (await axios.get("/api/account/bookings")).data;
        return { bookings: Array.isArray(apiReturn) ? apiReturn : [] };
    } catch (e) {
        console.error("Failed to fetch bookings:", e);
        return { bookings: [] };
    }
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
