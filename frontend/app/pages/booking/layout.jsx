import { Link, Outlet, useLocation } from "react-router";
import { BookingContext } from "./context";
import { useReducer } from "react";
import { authMiddleware } from "../../middleware/auth.middleware";

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
    const isRoundTrip = true;
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
            <FlightSubcard />
            {isRoundTrip && (
                <>
                    <div className="border" />
                    <FlightSubcard />
                </>
            )}
        </div>
    );
}

function FlightSubcard() {
    return (
        <div>
            <div className="flex justify-between bg-altitude-ink text-white p-2">
                <div>
                    <div>Return</div>
                    <div className="text-sm">June 30, 2026</div>
                </div>
                <div>Economy</div>
            </div>
            <div className="flex justify-between items-center mt-2">
                <div>
                    <div>03:45</div>
                    <div>MNL</div>
                </div>
                <span className="material-symbols-outlined">flight_takeoff</span>
                <div>
                    <div>09:00</div>
                    <div>CEB</div>
                </div>
            </div>
            <div className="text-center">Flight Details</div>
        </div>
    );
}
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

export default function BookingLayout() {
    const location = useLocation();
    const isInSeatmap = location.pathname === "/booking/details";
    const [bookingContext, setBookingContext] = useReducer(
        (state, action) => {
            console.log("before booking update: ", state);
            if (action.field == "passengers") {
                // console.log(state.passengers.length, action.count, !!action.count)
                if (action.count === null || action.count > state.passengers.length) {
                    console.error("On setBookingContext: Invalid Passenger Count passed.");
                    return state;
                } else if (!action.subField) {
                    console.error("On setBookingContext: No Subfield passed.");
                    return state;
                }
                let newPassengers;
                if (action.subField == "firstInitPassengerCount") {
                    newPassengers = [];
                    for (let i = 0; i < action.value.passengers; i++) newPassengers.push(structuredClone(passengerObject));
                    console.log("state at update:", action);
                    let ret = { ...state, passengers: newPassengers };
                    ret["flightId"] = action.value.flightId;
                    ret["departure_date"] = action.value.departure_date;
                    console.log("state after update:", ret);
                    return ret;
                } else {
                    newPassengers = structuredClone(state.passengers);
                    newPassengers[action.count][action.subField] = action.value;
                    let ret = { ...state, passengers: newPassengers };
                    return ret;
                }
            }
            let ret = { ...state, [action.field]: action.value };
            console.log("Update in bookingContext: ", ret);
            return ret;
        },
        {
            flightId: null,
            departure_date: null,
            passengers: [],
        },
    );
    return (
        <BookingContext value={{ ...bookingContext, setBookingContext: setBookingContext }}>
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
