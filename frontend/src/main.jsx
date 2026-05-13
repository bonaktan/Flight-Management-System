import { StrictMode } from "react";
import { createRoot } from "react-dom/client";
import "./index.css";

import { useState } from "react";
import reactLogo from "./assets/react.svg";
import viteLogo from "./assets/vite.svg";
import heroImg from "./assets/hero.png";
import "./App.css";

function App() {
    return <p className="bg-black text-white">tets</p>;
}

createRoot(document.getElementById("root")).render(
    <StrictMode>
        <App />
    </StrictMode>,
);
