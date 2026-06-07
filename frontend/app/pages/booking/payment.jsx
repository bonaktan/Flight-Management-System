import { NavLink, useNavigate } from "react-router";
import { InputField } from "../../components/input";
import { useState } from "react";

function PaymentSubsection({ name, selected, setSelected, children }) {
    return (
        <div>
            <div className="flex justify-between" onClick={() => setSelected(name)}>
                <div>{name}</div>
                <span className="material-symbols-outlined">{selected == name ? "arrow_drop_up" : "arrow_drop_down"}</span>
            </div>
            {selected == name && children}
        </div>
    );
}

export default function BookingPayment() {
    const navigate = useNavigate();
    const [selectedMethod, setSelectedMethod] = useState("");
    function onSubmitOnlineBanking() {
        // this should be a false redirect, we dont plan on getting actual money out of peeps
    }
    function onSubmitCreditCard() {
        navigate("/booking/confirmation");
    }
    return (
        <div>
            <div>Select your Payment Method</div>
            <div className="bg-orange-300">
                <PaymentSubsection name="Online Banking" selected={selectedMethod} setSelected={setSelectedMethod}>
                    <div>You will be redirected to the merchant's Website to complete the payment</div>
                    <div className="flex justify-center gap-2">
                        <div>GCash</div>
                        <div>Maya</div>
                        <div>PayPal</div>
                    </div>
                </PaymentSubsection>
                <PaymentSubsection name="Credit/Debit Card" selected={selectedMethod} setSelected={setSelectedMethod}>
                    <div className="flex justify-center gap-2">
                        <InputField name="Credit Card Number" icon="credit_card" />
                        <InputField name="Expiry Date" icon="calendar_today" />
                        <InputField name="CVV" icon="password" />
                    </div>
                    <button onClick={onSubmitCreditCard}>Next</button>
                </PaymentSubsection>
            </div>
        </div>
    );
}
