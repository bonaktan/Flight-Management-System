import { NavLink, useLocation, useNavigate } from "react-router";
import { InputField, SelectionField } from "../../components/input";
import { use, useEffect, useState } from "react";
import { BookingContext } from "./context";

export function meta() {
    return [{ title: "Booking - Passengers | SkyBridge Airlines" }];
}

function PassengerForm({ count, selectedPassenger, passenger, setPassenger }) {
    const bookingContext = use(BookingContext);
    const navigate = useNavigate();
    const [errors, setErrors] = useState({});

    function validate() {
        const e = {};
        if (!passenger.title) e.title = "Required";
        if (!passenger.first_name) e.first_name = "Required";
        else if (passenger.first_name.length > 64) e.first_name = "Max 64 characters";
        if (passenger.middle_name?.length > 64) e.middle_name = "Max 64 characters";
        if (!passenger.last_name) e.last_name = "Required";
        else if (passenger.last_name.length > 64) e.last_name = "Max 64 characters";
        if (!passenger.gender) e.gender = "Required";
        if (!passenger.date_of_birth) e.date_of_birth = "Required";
        else if (!/^\d{4}-(0[1-9]|1[0-2])-(0[1-9]|[12]\d|3[01])$/.test(passenger.date_of_birth)) e.date_of_birth = "Invalid date";
        if (!passenger.email) e.email = "Required";
        else if (!/^[^\s@]+@[^\s@]+\.[^\s@]+$/.test(passenger.email)) e.email = "Invalid email";
        else if (passenger.email.length > 254) e.email = "Max 254 characters";
        if (!passenger.phone_number) e.phone_number = "Required";
        else if (!/^\d{10,15}$/.test(passenger.phone_number)) e.phone_number = "10–15 digits only";
        if (!passenger.emergency_contact_name) e.emergency_contact_name = "Required";
        else if (passenger.emergency_contact_name.length > 128) e.emergency_contact_name = "Max 128 characters";
        if (!passenger.emergency_phone_number) e.emergency_phone_number = "Required";
        else if (!/^\d{10,15}$/.test(passenger.emergency_phone_number)) e.emergency_phone_number = "10–15 digits only";
        return e;
    }

    function onDropdownClick() {
        setPassenger(selectedPassenger == count ? -1 : count);
    }

    function onNext() {
        const e = validate();
        if (Object.keys(e).length > 0) {
            setErrors(e);
            return;
        }
        setErrors({});
        if (bookingContext.passengers.length == count + 1) {
            sessionStorage.setItem("bookingState", JSON.stringify(bookingContext));
            navigate("/booking/details");
        }
        setPassenger(selectedPassenger + 1);
    }

    const err = (field) => errors[field] && <span className="text-red-500 text-xs">{errors[field]}</span>;

    return (
        <div className="flex flex-col w-full bg-blaze-tint p-4 rounded-lg shadow-lg">
            <div className="flex border-b transition" onClick={onDropdownClick}>
                <p className="font-bold text-lg w-full">Customer Information - Passenger {count + 1}</p>
                <span className="material-symbols-outlined">{selectedPassenger == count ? "arrow_drop_up" : "arrow_drop_down"}</span>
            </div>
            {selectedPassenger == count && (
                <div>
                    <div className="flex flex-col w-full">
                        <span className="flex flex-row gap-2">
                            <div className="flex flex-col">
                                <SelectionField
                                    name="Title"
                                    value={passenger.title || ""}
                                    onChange={(e) =>
                                        bookingContext.setBookingContext({ field: "passengers", count, subField: "title", value: e.target.value })
                                    }>
                                    <option value="" disabled>
                                        -Select Title-
                                    </option>
                                    <option value="mr">Mr.</option>
                                    <option value="ms">Ms.</option>
                                    <option value="mrs">Mrs.</option>
                                    <option value="dr">Dr.</option>
                                </SelectionField>
                                {err("title")}
                            </div>
                            <div className="flex flex-col">
                                <InputField
                                    name="First Name"
                                    type="text"
                                    value={passenger.first_name}
                                    onChange={(e) =>
                                        bookingContext.setBookingContext({
                                            field: "passengers",
                                            count,
                                            subField: "first_name",
                                            value: e.target.value,
                                        })
                                    }
                                />
                                {err("first_name")}
                            </div>
                            <div className="flex flex-col">
                                <InputField
                                    name="Last Name"
                                    type="text"
                                    value={passenger.last_name}
                                    onChange={(e) =>
                                        bookingContext.setBookingContext({ field: "passengers", count, subField: "last_name", value: e.target.value })
                                    }
                                />
                                {err("last_name")}
                            </div>
                        </span>
                        <span className="flex flex-row gap-4 items-start">
                            <div className="flex gap-2 items-center justify-center h-full">
                                {["Male", "Female"].map((gender) => (
                                    <label key={gender} className="flex items-center gap-1">
                                        <input
                                            type="radio"
                                            name="gender"
                                            value={gender.toLowerCase()}
                                            checked={passenger.gender == gender.toLowerCase()}
                                            onChange={(e) =>
                                                bookingContext.setBookingContext({
                                                    field: "passengers",
                                                    count,
                                                    subField: "gender",
                                                    value: e.target.value,
                                                })
                                            }
                                        />
                                        {gender}
                                    </label>
                                ))}
                                {err("gender")}
                            </div>
                            <div className="flex flex-col">
                                <InputField
                                    name="Date of Birth"
                                    type="date"
                                    value={passenger.date_of_birth}
                                    onChange={(e) =>
                                        bookingContext.setBookingContext({
                                            field: "passengers",
                                            count,
                                            subField: "date_of_birth",
                                            value: e.target.value,
                                        })
                                    }
                                />
                                {err("date_of_birth")}
                            </div>
                        </span>
                        <p className="font-bold text-lg w-full border-b">Contact Information</p>
                        <span className="flex flex-row gap-4">
                            <div className="flex flex-col">
                                <InputField
                                    name="Phone Number"
                                    type="tel"
                                    value={passenger.phone_number}
                                    onChange={(e) =>
                                        bookingContext.setBookingContext({
                                            field: "passengers",
                                            count,
                                            subField: "phone_number",
                                            value: e.target.value,
                                        })
                                    }
                                />
                                {err("phone_number")}
                            </div>
                            <div className="flex flex-col">
                                <InputField
                                    name="Email"
                                    type="email"
                                    value={passenger.email}
                                    onChange={(e) =>
                                        bookingContext.setBookingContext({ field: "passengers", count, subField: "email", value: e.target.value })
                                    }
                                />
                                {err("email")}
                            </div>
                        </span>
                        <p className="font-bold text-lg w-full border-b">Emergency Contact</p>
                        <span className="flex flex-row gap-4">
                            <div className="flex flex-col">
                                <InputField
                                    name="Emergency Contact Name"
                                    type="text"
                                    value={passenger.emergency_contact_name}
                                    onChange={(e) =>
                                        bookingContext.setBookingContext({
                                            field: "passengers",
                                            count,
                                            subField: "emergency_contact_name",
                                            value: e.target.value,
                                        })
                                    }
                                />
                                {err("emergency_contact_name")}
                            </div>
                            <div className="flex flex-col">
                                <InputField
                                    name="Emergency Contact Phone"
                                    type="tel"
                                    value={passenger.emergency_phone_number}
                                    onChange={(e) =>
                                        bookingContext.setBookingContext({
                                            field: "passengers",
                                            count,
                                            subField: "emergency_phone_number",
                                            value: e.target.value,
                                        })
                                    }
                                />
                                {err("emergency_phone_number")}
                            </div>
                        </span>
                    </div>
                    <div className="flex justify-end">
                        <button onClick={onNext}>Next</button>
                    </div>
                </div>
            )}
        </div>
    );
}

export default function BookingForm() {
    const { state } = useLocation();
    const bookingState = use(BookingContext);

    useEffect(() => {
        const savedState = sessionStorage.getItem("pendingNavState");
        const resolvedState = state ?? (savedState ? JSON.parse(savedState) : null);

        if (savedState) sessionStorage.removeItem("pendingNavState");

        if (resolvedState != null && bookingState.passengers.length != resolvedState.passengers)
            bookingState.setBookingContext({ field: "passengers", count: 0, subField: "firstInitPassengerCount", value: resolvedState });
    }, []);

    const [passengerSelected, setPassengerSelected] = useState(0);
    return (
        <div className="p-2 w-full">
            <p className="text-2xl font-bold">Passenger Information</p>
            <p>Please enter the information of the passengers that will board this flight.</p>
            <div className="flex flex-col gap-4 mx-4 my-8">
                {bookingState.passengers.map((passenger, i) =>
                    passenger == null ? (
                        <div key={i}>Loading...</div>
                    ) : (
                        <PassengerForm
                            key={i}
                            count={i}
                            selectedPassenger={passengerSelected}
                            passenger={passenger}
                            setPassenger={setPassengerSelected}
                        />
                    ),
                )}
            </div>
        </div>
    );
}
