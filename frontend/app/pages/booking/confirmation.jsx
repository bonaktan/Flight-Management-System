import { NavLink } from "react-router";

export default function BookingConfirmation() {
    return (
        <div className="flex justify-center items-center">
            <div className="flex flex-col items-center justify-center bg-gray-100 rounded-xl m-10 w-fit p-16 shadow-md ">
                <div className="relative flex items-center justify-center w-48 h-48 p-8 bg-emerald-400 rounded-full shadow-md ">
                    <svg className="w-full h-full" fill="#000000" viewBox="0 0 1920 1920" xmlns="http://www.w3.org/2000/svg">
                        <path d="M1827.701 303.065 698.835 1431.801 92.299 825.266 0 917.564 698.835 1616.4 1919.869 395.234z" fill-rule="evenodd" />
                    </svg>
                </div>

                {/* Confirmation Text */}
                <div className="text-center mt-6">
                    <h3 className="text-xl font-semibold">Action Successful</h3>
                    <p className="">Your changes have been saved completely. You're good to go!</p>
                </div>
                <div className="mt-4">
                    <NavLink className="px-4 py-2 bg-blaze-core text-cloud-warm font-semibold text-xl rounded-md " to="/account/booking">
                        Go to Bookings
                    </NavLink>
                </div>
            </div>
        </div>
    );
}
