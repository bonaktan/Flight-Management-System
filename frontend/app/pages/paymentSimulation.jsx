import { useEffect, useState } from "react";

const COUNTDOWN = 3;
export function meta() {
    return [{ title: "Payment Provider Simulation :3" }];
}
// It simulates a payment processor UI: counts down, then fires postMessage back to the opener.
export default function PaymentSimulation() {
    const [secondsLeft, setSecondsLeft] = useState(COUNTDOWN);
    useEffect(() => {
        if (secondsLeft <= 0) {
            const payload = {
                processedAt: new Date().toISOString(),
                gatewayRef: "GW-" + Math.random().toString(36).substr(2, 8).toUpperCase(),
            };
            if (window.opener && !window.opener.closed) {
                window.opener.postMessage({ type: "PAYMENT_COMPLETE", payload }, window.location.origin);
            }
            return;
        }
        const timer = setTimeout(() => setSecondsLeft((s) => s - 1), 1000);
        return () => clearTimeout(timer);
    }, [secondsLeft]);

    return (
        <div>
            <h2>Processing payment...</h2>
        </div>
    );
}
