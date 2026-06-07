import { Link, Outlet, useLocation } from "react-router";
import BookingForm from "./form";
import { BookingContext } from "./context";
import { useReducer } from "react";

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
        <div>
            <div className="flex justify-between">
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
            <div className="flex justify-between">
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

export default function BookingLayout() {
    const [bookingContext, setBookingContext] = useReducer(
        (state, action) => {
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
                    for (let i = 0; i < action.value; i++) newPassengers.push(structuredClone(newPassengers[0]));
                } else {
                    newPassengers = structuredClone(state.passengers);
                    newPassengers[action.count][action.subField] = action.value;
                }
                return { ...state, passengers: newPassengers };
            }
            return { ...state, [action.field]: action.value };
        },
        {
            flightId: null,
            flightClass: null,
            passengers: [
                {
                    title: "",
                    first_name: "",
                    middle_name: "",
                    last_name: "",
                    gender: null,
                    date_of_birth: "",
                    email: "",
                    phone_number: "",
                    emergency_contact_name: "",
                    emergency_phone_number: "",
                    selected_seat: "",
                },
            ],
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
                    <HeaderLogos logo="credit_card" label="Payment" page="/booking/confirmation" />
                </div>
                <div className="flex gap-8">
                    <div className="w-full">
                        <Outlet />
                    </div>
                    {/* <div className=" m-8">
                        {/* <BillingCard /> 
                    </div> */}
                </div>
            </div>
        </BookingContext>
    );
}
