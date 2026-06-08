import { NavLink } from "react-router";
import { InputField, SelectionField } from "../../components/input";

export default function BookingForm() {
    return (
        <div className="flex flex-col md:flex-row gap-4 p-2">
            <div className="flex flex-col w-full md:w-1/2 bg-blaze-tint p-4 rounded-lg shadow-lg">
                <p className="font-bold text-lg w-full border-b">Customer Information</p>
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
            </div>
        </div>
    );
}
