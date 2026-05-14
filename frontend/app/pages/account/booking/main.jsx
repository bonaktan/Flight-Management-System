import { NavLink } from "react-router";

export default function AccountBooking() {
    return (
        <div>
            <p>Account - My Bookings</p>
            {Object.entries({ PH6769: {} }).map(([flightID, data]) => (
                <div className="flex justify-between" key={flightID}>
                    <p>{flightID}</p>
                    <NavLink to="/account/booking/details">Details</NavLink>
                </div>
            ))}
        </div>
    );
}
