import { NavLink, useNavigate } from "react-router";
import { InputField } from "../../components/input";
import { use, useRef, useState } from "react";
import { BookingContext } from "./context";
import axios from "axios";

export function meta() {
    return [{ title: "Booking - Payment | SkyBridge Airlines" }];
}
export function HydrateFallback() {
    return (
        <div className="flex justify-center items-center">
            <p>Loading...</p>
        </div>
    );
}
export async function clientLoader() {
    // Opt into a fallback while server loader runs
    return;
}
clientLoader.hydrate = true;
const apiUrl = import.meta.env.VITE_BACKEND_URL;

function PaymentSubsection({ name, selected, setSelected, setError, children }) {
    return (
        <div className="bg-cloud-warm text-altitude-ink p-4 rounded-lg">
            <div
                className="flex justify-between text-xl font-bold"
                onClick={() => {
                    (setSelected(name), setError(""));
                }}>
                <div>{name}</div>
                <span className="material-symbols-outlined">{selected === name ? "arrow_drop_up" : "arrow_drop_down"}</span>
            </div>
            <div
                className={`transition-all duration-500 ease-in-out overflow-hidden ${selected === name ? "max-h-60 opacity-100" : "max-h-0 opacity-0"}`}>
                {children}
            </div>
        </div>
    );
}

// Waits for a postMessage with type "PAYMENT_COMPLETE" from the popup window.
// Resolves with the message payload, or rejects if the popup is closed manually.
function waitForPopupMessage(win) {
    return new Promise((resolve, reject) => {
        function onMessage(event) {
            // if (event.origin !== "https://yoursite.com") return;
            if (event.data?.type === "PAYMENT_COMPLETE") {
                cleanup();
                resolve(event.data);
            }
        }
        function onInterval() {
            if (win.closed) {
                cleanup();
                reject(new Error("Popup closed by user"));
            }
        }
        const interval = setInterval(onInterval, 500);

        function cleanup() {
            window.removeEventListener("message", onMessage);
            clearInterval(interval);
        }
        window.addEventListener("message", onMessage);
    });
}
function luhnsAlgorithm(cardNumber) {
    // accept only digits, dashes or spaces
    if (!cardNumber || /[^0-9-\s]+/.test(cardNumber)) return false;
    let nCheck = 0;
    let bEven = false;
    cardNumber = cardNumber.replace(/\D/g, "");
    for (let n = cardNumber.length - 1; n >= 0; n--) {
        let cDigit = cardNumber.charAt(n);
        let nDigit = parseInt(cDigit, 10);

        if (bEven) {
            if ((nDigit *= 2) > 9) nDigit -= 9;
        }
        nCheck += nDigit;
        bEven = !bEven;
    }
    return nCheck % 10 == 0;
}

async function backendSubmit(payload) {
    const response = await axios.post(`/api/booking/submit`, {
        flightId: payload.flightId,
        passengers: payload.passengers,
        departure_date: payload.departure_date,
    });
    return response;
}
export default function BookingPayment() {
    const navigate = useNavigate();
    const [selectedMethod, setSelectedMethod] = useState("");
    const bookingContext = use(BookingContext);
    const [paymentStatus, setPaymentStatus] = useState(null);
    const [paymentError, setPaymentError] = useState(null);
    const [cardNumber, setCardNumber] = useState("");
    const paymentPopupRef = useRef(null);
    async function onSubmitOnlineBanking(e) {
        // this is a false popup, we dont plan on getting actual money out of peeps
        e.preventDefault();
        setPaymentStatus("waiting");
        try {
            // sessionStorage.setItem("booking_state", JSON.stringify(bookingContext))
            const win = window.open("/payment/simulation", "payment_popup", "width=480,height=600,left=200,top=100");
            if (!win) throw new Error("Popup was blocked. Allow popups for this site.");
            paymentPopupRef.current = win;
            const result = await waitForPopupMessage(win);
            if (!win.closed) win.close();
            setPaymentStatus("success");
            // JSON.parse(sessionStorage.getItem("payment_state") || "{}")
            // TODO: send the details to the backend
            await backendSubmit(bookingContext);
            sessionStorage.removeItem("bookingState");
            navigate("/booking/confirmation");
        } catch (err) {
            console.log(err);
            setPaymentError(err.response.data.error);
            setPaymentStatus("error");
        } finally {
            paymentPopupRef.current = null;
        }
    }

    async function onSubmitCard() {
        // this is also a false popup, i DONT want to store actual credit card numbers
        // though, luhns algorithm is too interesting of an algo to not include
        if (!luhnsAlgorithm(cardNumber)) {
            setPaymentError("Invalid Credit Card Number");
            return;
        }
        try {
            await backendSubmit(bookingContext);
            sessionStorage.removeItem("bookingState");
            navigate("/booking/confirmation");
        } catch (err) {
            console.log(err);
            setPaymentError(err.response.data);
        }
    }
    return (
        <div>
            <div className="text-2xl text-altitude-ink font-bold mx-10 my-4">Select your Payment Method</div>
            <div className="bg-blaze-core m-4 p-4 flex flex-col gap-2 rounded-xl">
                <PaymentSubsection name="Online Banking" selected={selectedMethod} setSelected={setSelectedMethod} setError={setPaymentError}>
                    <div>You will be redirected to the merchant's Website to complete the payment</div>
                    <div className="flex justify-center gap-2 mt-4">
                        <button className="bg-blaze-deep text-white px-4 py-2 rounded-md" onClick={onSubmitOnlineBanking}>
                            GCash
                        </button>
                        <button className="bg-blaze-deep text-white px-4 py-2 rounded-md" onClick={onSubmitOnlineBanking}>
                            Maya
                        </button>
                        <button className="bg-blaze-deep text-white px-4 py-2 rounded-md" onClick={onSubmitOnlineBanking}>
                            PayPal
                        </button>
                    </div>
                    <div className="text-center text-red-500">{paymentError}</div>
                </PaymentSubsection>
                <PaymentSubsection name="Credit/Debit Card" selected={selectedMethod} setSelected={setSelectedMethod} setError={setPaymentError}>
                    <div>Please enter you Card Information.</div>
                    <div className="flex justify-center gap-2">
                        <InputField name="Credit Card Number" icon="credit_card" value={cardNumber} onChange={(e) => setCardNumber(e.target.value)} />
                        <InputField name="Expiry Date" icon="calendar_today" />
                        <InputField name="CVV" icon="password" />
                    </div>
                    <div className="flex justify-center mt-4">
                        <button className="bg-blaze-deep text-white px-4 py-2 rounded-md" onClick={onSubmitCard}>
                            Submit
                        </button>
                    </div>
                    <div className="text-center text-red-500">{paymentError}</div>
                </PaymentSubsection>
            </div>
        </div>
    );
}
