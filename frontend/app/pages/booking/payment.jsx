import { NavLink, useNavigate } from "react-router";
import { InputField } from "../../components/input";
import { use, useRef, useState } from "react";
import { BookingContext } from "./context";

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

// Waits for a postMessage with type "PAYMENT_COMPLETE" from the popup window.
// Resolves with the message payload, or rejects if the popup is closed manually.
function waitForPopupMessage(win) {
    return new Promise((resolve, reject) => {
        function onMessage(event) {
            // In production, restrict origin:
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
    if (/[^0-9-\s]+/.test(cardNumber)) return false;
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
            console.log(result);
            setPaymentStatus("success");
            // JSON.parse(sessionStorage.getItem("payment_state") || "{}")
            // TODO: send the details to the backend
            navigate("/booking/confirmation");
        } catch (err) {
            console.log(err);
            setPaymentError(err.message);
            setPaymentStatus("error");
        } finally {
            paymentPopupRef.current = null;
        }
    }

    function onSubmitCard() {
        // this is also a false popup, i DONT want to store actual credit card numbers
        // though, luhns algorithm is too interesting of an algo to not include
        if (!luhnsAlgorithm(cardNumber)) {
            setPaymentError("Invalid Credit Card Number");
            return;
        }
        navigate("/booking/confirmation");
    }
    return (
        <div>
            <div className="text-3xl text-center text-altitude-ink font-bold pb-2 border-b-1 border-cloud-pop my-2">Select your Payment Method</div>
            <div className="relative bg-cloud-warm flex border-1 border-cloud-pop m-2">
                {["E-wallet", "Credit/Debit Card"].map((method) => (
                    <button
                        key={method}
                        className={`w-full font-bold p-4 z-1 transition ${selectedMethod === method ? "text-white" : "text-altitude-ink"}`}
                        onClick={() => setSelectedMethod(method)}>
                        {method}
                    </button>
                ))}
                <div
                    className="absolute top-0 h-full w-1/2 bg-blaze-deep transition z-0"
                    style={{ left: selectedMethod === "Credit/Debit Card" ? "50%" : "0%" }}
                />
            </div>
            <div>{paymentError}</div>
        </div>
    );
}
