import { Outlet } from "react-router";

export default function AccountBookingLayout() {
    return (
        <div>
            <div className="text-2xl font-bold">Booking</div>
            <Outlet />
        </div>
    );
}
