import { StrictMode } from "react";
import { createRoot } from "react-dom/client";
import "./App.css";
import "./index.css";

function App() {
    return <p className="bg-black text-white">tets</p>;
}

createRoot(document.getElementById("root")).render(
    <StrictMode>
        <App />
    </StrictMode>,
);
