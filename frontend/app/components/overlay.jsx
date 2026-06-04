import { useContext } from "react";
import OverlayContext from "./context/overlayContext";

export function OverlayBase({ children, open, setOpen }) {
    return (
        <OverlayContext.Provider value={{ open, setOpen }}>
            <div
                className={`fixed top-0 left-0 w-screen h-screen bg-black/30 z-100 ${open ? "block" : "hidden"}`}
                onClick={(e) => {
                    e.stopPropagation();
                    setOpen(false);
                }}>
                {children}
            </div>
        </OverlayContext.Provider>
    );
}

export function OverlaySidebar({ children, className }) {
    const { open, setOpen } = useContext(OverlayContext);
    return (
        <div
            className={`absolute flex flex-col bg-white h-full w-1/3 ${className || ""}`}
            onClick={(e) => {
                e.stopPropagation();
            }}>
            <div className="flex flex-row justify-end p-2">
                <button
                    className=""
                    onClick={(e) => {
                        e.stopPropagation();
                        setOpen(false);
                    }}>
                    X
                </button>
            </div>
            {children}
        </div>
    );
}

export function OverlayModal({ children, className }) {
    const { open, setOpen } = useContext(OverlayContext);
    return (
        <div
            className={`absolute top-1/2 left-1/2 transform -translate-x-1/2 -translate-y-1/2 flex flex-col bg-white max-h-3/4 w-1/3 ${className || ""}`}
            onClick={(e) => {
                e.stopPropagation();
            }}>
            <div className="flex flex-row justify-end p-2">
                <button
                    className=""
                    onClick={(e) => {
                        e.stopPropagation();
                        setOpen(false);
                    }}>
                    X
                </button>
            </div>
            {children}
        </div>
    );
}
