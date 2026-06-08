import { NavLink, useLocation, useNavigate } from "react-router";
import { InputField, SelectionField } from "../../components/input";
import { use, useEffect, useState } from "react";
import { BookingContext } from "./context";

function PassengerForm({ count, selectedPassenger, passenger, setPassenger }) {
    const bookingContext = use(BookingContext);
    const navigate = useNavigate();
    function onDropdownClick() {
        let newValue;
        if (selectedPassenger == count) newValue = -1;
        else newValue = count;
        setPassenger(newValue);
    }
    function onNext() {
        if (bookingContext.passengers.length == count + 1) {
            sessionStorage.setItem("bookingState", JSON.stringify(bookingContext));
            navigate("/booking/details");
        }
        setPassenger(selectedPassenger + 1);
    }
    return (
        <div className="flex flex-col w-full bg-blaze-tint p-4 rounded-lg shadow-lg">
            <div className="flex border-b" onClick={onDropdownClick}>
                <p className="font-bold text-lg w-full ">Customer Information - Passenger {count + 1}</p>
                <span className="material-symbols-outlined">{selectedPassenger == count ? "arrow_drop_up" : "arrow_drop_down"}</span>
            </div>
            {selectedPassenger == count && (
                <div>
                    <div className="flex flex-col w-full">
                        <span className="flex flex-row gap-2">
                            <SelectionField
                                name="Title"
                                value={passenger.title || ""}
                                onChange={(e) =>
                                    bookingContext.setBookingContext({ field: "passengers", count: count, subField: "title", value: e.target.value })
                                }>
                                <option value="" disabled>
                                    -Select Title-
                                </option>
                                <option value="mr">Mr.</option>
                                <option value="ms">Ms.</option>
                                <option value="mx">Mx.</option>
                            </SelectionField>
                            <InputField
                                name="First Name"
                                type="text"
                                value={passenger.first_name}
                                onChange={(e) =>
                                    bookingContext.setBookingContext({
                                        field: "passengers",
                                        count: count,
                                        subField: "first_name",
                                        value: e.target.value,
                                    })
                                }
                            />
                            <InputField
                                name="Last Name"
                                type="text"
                                value={passenger.last_name}
                                onChange={(e) =>
                                    bookingContext.setBookingContext({
                                        field: "passengers",
                                        count: count,
                                        subField: "last_name",
                                        value: e.target.value,
                                    })
                                }
                            />
                        </span>
                        <span className="flex flex-row gap-4">
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
                                                count: count,
                                                subField: "gender",
                                                value: e.target.value,
                                            })
                                        }
                                    />
                                    {gender}
                                </label>
                            ))}
                            <InputField
                                name="Date of Birth"
                                type="date"
                                value={passenger.date_of_birth}
                                onChange={(e) =>
                                    bookingContext.setBookingContext({
                                        field: "passengers",
                                        count: count,
                                        subField: "date_of_birth",
                                        value: e.target.value,
                                    })
                                }
                            />
                        </span>
                        <p className="font-bold text-lg w-full border-b">Contact Information</p>
                        <span className="flex flex-row gap-4">
                            <InputField
                                name="Phone Number"
                                type="tel"
                                value={passenger.phone_number}
                                onChange={(e) =>
                                    bookingContext.setBookingContext({
                                        field: "passengers",
                                        count: count,
                                        subField: "phone_number",
                                        value: e.target.value,
                                    })
                                }
                            />
                            <InputField
                                name="Email"
                                type="email"
                                value={passenger.email}
                                onChange={(e) =>
                                    bookingContext.setBookingContext({
                                        field: "passengers",
                                        count: count,
                                        subField: "email",
                                        value: e.target.value,
                                    })
                                }
                            />
                        </span>
                        <p className="font-bold text-lg w-full border-b">Emergency Contact</p>
                        <span className="flex flex-row gap-4">
                            <InputField
                                name="Emergency Contact Name"
                                type="text"
                                value={passenger.emergency_contact_name}
                                onChange={(e) =>
                                    bookingContext.setBookingContext({
                                        field: "passengers",
                                        count: count,
                                        subField: "emergency_contact_name",
                                        value: e.target.value,
                                    })
                                }
                            />
                            <InputField
                                name="Emergency Contact Phone"
                                type="tel"
                                value={passenger.emergency_phone_number}
                                onChange={(e) =>
                                    bookingContext.setBookingContext({
                                        field: "passengers",
                                        count: count,
                                        subField: "emergency_phone_number",
                                        value: e.target.value,
                                    })
                                }
                            />
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
            bookingState.setBookingContext({ field: "passengers", count: 0, subField: "firstInitPassengerCount", value: state });
        console.log("bookingState after init: ", bookingState);
        console.log("state passed to bookingInit: ", state);
    }, []);

    const [passengerSelected, setPassengerSelected] = useState(0);
    return (
        <div className="p-2 w-full">
            <p className="text-2xl font-bold">Passenger Information</p>
            <p>Please enter the information of the passengers that will board this flight.</p>
            <div className="flex flex-col gap-4 mx-4 my-8">
                {bookingState.passengers.map((key, i) =>
                    key == null ? (
                        <div key={i}>Loading...</div>
                    ) : (
                        <PassengerForm key={i} count={i} selectedPassenger={passengerSelected} passenger={key} setPassenger={setPassengerSelected} />
                    ),
                )}
            </div>
        </div>
    );
}
