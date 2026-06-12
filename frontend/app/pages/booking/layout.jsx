import { Link, Outlet, useLocation } from "react-router";
import { BookingContext } from "./context";
import { use, useEffect, useReducer, useState } from "react";
import { authMiddleware } from "../../middleware/auth.middleware";
import { FlightCard } from "../../components/flightCard";
export const middleware = [authMiddleware];

function HeaderLogos({ logo, label, page }) {
    return (
        <Link to={`${page}`} className="flex gap-4 items-center">
            <span className="material-symbols-outlined bg-white p-1 md:p-3 rounded-full">{logo}</span>
            <div className="md:flex hidden">{label}</div>
        </Link>
    );
}
function BillingCard() {
    const bookingContext = use(BookingContext);
    const isRoundTrip = false; // WARN: change to true once implemented na yung ano
    return (
        <div className="">
            <div className="flex justify-between bg-blaze-deep text-white p-2">
                <div>
                    <div>Total to be paid</div>
                    <div className="text-xs">Excluding taxes, fees, and discounts</div>
                </div>
                <div>PHP 6,696.96</div>
            </div>
            <div className="border"></div>
            <FlightCard flight={bookingContext.selectedFlight} />
            {isRoundTrip && (
                <>
                    <div className="border" />
                    <FlightCard />
                </>
            )}
        </div>
    );
}

// function FlightSubcard() {
//     return (
//         <div>
//             <div className="flex justify-between bg-altitude-ink text-white p-2">
//                 <div>
//                     <div>Return</div>
//                     <div className="text-sm">June 30, 2026</div>
//                 </div>
//                 <div>Economy</div>
//             </div>
//             <div className="flex justify-between items-center mt-2">
//                 <div>
//                     <div>03:45</div>
//                     <div>MNL</div>
//                 </div>
//                 <span className="material-symbols-outlined">flight_takeoff</span>
//                 <div>
//                     <div>09:00</div>
//                     <div>CEB</div>
//                 </div>
//             </div>
//             <div className="text-center">Flight Details</div>
//         </div>
//     );
// }
const passengerObject = {
    title: "",
    first_name: "",
    middle_name: "",
    last_name: "",
    gender: "",
    date_of_birth: "",
    email: "",
    phone_number: "",
    emergency_contact_name: "",
    emergency_phone_number: "",
    selected_seat: "",
};

export async function clientLoader() {
    const mockApi = {
        airplane_id: "SB-W0001",
        departure: "2026-06-30 15:00:00",
        destination: "CEB",
        flightId: "SKY067",
        flight_time: 5400,
        model: "Airbus A320",
        origin: "MNL",
    };
    return mockApi;
}
export function HydrateFallback() {
    return (
        <div className="flex justify-center items-center">
            <p>Loading...</p>
        </div>
    );
}
export default function BookingLayout({ loaderData }) {
    const location = useLocation();
    const isInSeatmap = location.pathname === "/booking/details";

    // Restore on mount
    const [bookingContext, setBookingContext] = useReducer(
        (state, action) => {
            if (action.field == "passengers") {
                if (!action.subField) {
                    console.error("On setBookingContext: No Subfield passed.");
                    return state;
                }
                if (action.subField !== "firstInitPassengerCount" && (action.count == null || action.count >= state.passengers.length)) {
                    console.error("On setBookingContext: Invalid Passenger Count passed.");
                    return state;
                }
                let newPassengers;
                if (action.subField == "firstInitPassengerCount") {
                    newPassengers = [];
                    for (let i = 0; i < action.value.passengers; i++) newPassengers.push(structuredClone(passengerObject));
                    let ret = { ...state, passengers: newPassengers };
                    ret["flightId"] = action.value.flightId;
                    ret["departure_date"] = action.value.departure_date;
                    return ret;
                } else {
                    newPassengers = structuredClone(state.passengers);
                    newPassengers[action.count][action.subField] = action.value;
                    let ret = { ...state, passengers: newPassengers };
                    return ret;
                }
            }
            let ret = { ...state, [action.field]: action.value };
            return ret;
        },
        (() => {
            if (typeof window !== "undefined") {
                const saved = sessionStorage.getItem("bookingState");
                if (saved) {
                    try {
                        return JSON.parse(saved);
                    } catch (e) {
                        console.error("Failed to parse bookingState", e);
                    }
                }
            }
            return {
                flightId: null,
                departure_date: null,
                passengers: [],
            };
        })(),
    );
    useEffect(() => {
        const handleBeforeUnload = () => {
            sessionStorage.setItem("bookingState", JSON.stringify(bookingContext));
        };
        window.addEventListener("beforeunload", handleBeforeUnload);
        return () => window.removeEventListener("beforeunload", handleBeforeUnload);
    }, [bookingContext]);
    return (
        <BookingContext value={{ ...bookingContext, setBookingContext: setBookingContext, selectedFlight: loaderData }}>
            <div>
                <div id="steps" className="flex items-center justify-center gap-4 w-full bg-orange-300 p-2">
                    <HeaderLogos logo="flight_takeoff" label="Flights" page="/search" />
                    <span className="material-symbols-outlined">keyboard_double_arrow_right</span>
                    <HeaderLogos logo="person" label="Passengers" page="/booking/form" />
                    <span className="material-symbols-outlined">keyboard_double_arrow_right</span>
                    <HeaderLogos logo="airline_seat_recline_extra" label="Seats" page="/booking/details" />
                    <span className="material-symbols-outlined">keyboard_double_arrow_right</span>
                    <HeaderLogos logo="credit_card" label="Payment" page="/booking/payment" />
                </div>
                <div className="flex gap-8">
                    <div className={`${isInSeatmap ? "w-full" : "w-3/4"}`}>
                        <Outlet />
                    </div>

                    {!isInSeatmap && (
                        <div className="m-8">
                            <BillingCard />
                        </div>
                    )}
                </div>
            </div>
        </BookingContext>
    );
}
