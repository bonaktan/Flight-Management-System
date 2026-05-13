import { NavLink } from "react-router";

export default function BookingPayment() {
    return (
        <div>
            <p>Booking - Payment (Step 3)</p>
            <NavLink to="/booking/confirmation">Next</NavLink>
        </div>
    );
}
