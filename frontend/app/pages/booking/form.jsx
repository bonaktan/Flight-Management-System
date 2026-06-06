import { NavLink, useLocation, useNavigate } from "react-router";
import { InputField, SelectionField } from "../../components/input";
import { use, useEffect, useState } from "react";
import { BookingContext } from "./context";

function PassengerForm({ count, passenger, setPassenger }) {
    const bookingContext = use(BookingContext);
    const navigate = useNavigate();
    function onDropdownClick() {
        let newValue;
        if (passenger == count) newValue = -1;
        else newValue = count;
        setPassenger(newValue);
    }
    function onNext() {
        if (bookingContext.passengers.length == count + 1) navigate("/booking/details");
        setPassenger(passenger + 1);
    }
    return (
        <div className="flex flex-col w-full bg-blaze-tint p-4 rounded-lg shadow-lg">
            <div className="flex border-b" onClick={onDropdownClick}>
                <p className="font-bold text-lg w-full ">Customer Information - Passenger {count + 1}</p>
                <span class="material-symbols-outlined">{passenger == count ? "arrow_drop_up" : "arrow_drop_down"}</span>
            </div>
            {passenger == count && (
                <div>
                    <div className="flex flex-col w-full">
                        <span className="flex flex-row gap-2">
                            <SelectionField name="Title">
                                <option value="" disabled selected>
                                    -Select Title-
                                </option>
                                <option value="mr">Mr.</option>
                                <option value="ms">Ms.</option>
                                <option value="mx">Mx.</option>
                            </SelectionField>
                            <InputField name="First Name" type="text" />
                            <InputField name="Last Name" type="text" />
                        </span>
                        <span className="flex flex-row gap-4">
                            {["Male", "Female"].map((gender) => (
                                <label key={gender} className="flex items-center gap-1">
                                    <input type="radio" name="gender" value={gender.toLowerCase()} />
                                    {gender}
                                </label>
                            ))}
                            <InputField name="Date of Birth" type="date" />
                        </span>
                        <p className="font-bold text-lg w-full border-b">Contact Information</p>
                        <span className="flex flex-row gap-4">
                            <InputField name="Phone Number" type="tel" />
                            <InputField name="Email" type="email" />
                        </span>
                        <p className="font-bold text-lg w-full border-b">Emergency Contact</p>
                        <span className="flex flex-row gap-4">
                            <InputField name="Emergency Contact Name" type="text" />
                            <InputField name="Emergency Contact Phone" type="tel" />
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
        if (state != null && bookingState.passengers.length != state.passengers)
            bookingState.setBookingContext({ field: "passengers", count: 0, subField: "firstInitPassengerCount", value: parseInt(state.passengers) });
        console.log(bookingState);
        console.log(state);
    }, []);

    const [passengerSelected, setPassengerSelected] = useState(0);
    return (
        <div className="p-2 w-full">
            <p className="text-2xl font-bold">Passenger Information</p>
            <p>Please enter the information of the passengers that will board this flight.</p>
            <div className="flex flex-col gap-4 mx-4 my-8">
                {bookingState.passengers.map((key, i) => (
                    <PassengerForm key={i} count={i} passenger={passengerSelected} setPassenger={setPassengerSelected} />
                ))}
            </div>
        </div>
    );
}
