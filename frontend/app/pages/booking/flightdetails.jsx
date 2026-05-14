import { NavLink } from "react-router";

export default function BookingFlightDetails() {
    return (
        <div>
            <p>Booking - Flight Details (Step 2)</p>
            <NavLink to="/booking/payment">Next</NavLink>
        </div>
    );
}
