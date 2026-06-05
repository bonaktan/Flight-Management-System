import { useReducer, useState } from "react";
import "./stylesheets/selection.css";

export function InputField({ name, icon, genDesign, inDesign, labDesign, ...props }) {
    return (
        <div className={`flex flex-col ${genDesign}`}>
            <label className={`${labDesign}`}>{name}</label>
            <div className={`h-10 p-1 border flex gap-1 items-center rounded-sm w-full ${inDesign}`}>
                <p className="material-symbols-outlined select-none">{icon}</p>
                <input className={`w-full focus:outline-none ${inDesign}`} {...props} name={name} />
            </div>
        </div>
    );
}

export function SelectionField({ name, children, genDesign, selDesign, labDesign, ...props }) {
    return (
        <div className={`flex flex-col ${genDesign}`}>
            <label className={labDesign}>{name}</label>
            <select id="" className={`h-10 border border-black rounded-sm w-full px-2 ${selDesign}`} name={name} {...props}>
                {children}
            </select>
        </div>
    );
}

export function CounterField({ name, genDesign, labDesign, min = null, max = null, onChange = () => {}, value, defaultValue = 1 }) {
    const [_, setCount] = useReducer((state, action) => {
        const newState = state + action;
        if (min && max && (newState < min || newState > max)) return state;
        onChange(newState);
        return newState;
    }, defaultValue);
    if (!value) value = _;

    return (
        <div className={`flex flex-col ${genDesign}`}>
            <label className={labDesign}>{name}</label>
            <div className="flex flex-row gap-2 items-center">
                <button type="button" onClick={() => setCount(-1)} className="text-2xl my-1 h-8 px-2 rounded-full border border-black">
                    -
                </button>
                <p className="w-6 text-center select-none">{value}</p>
                <button type="button" onClick={() => setCount(1)} className="text-2xl my-1 h-8 px-2 rounded-full border border-black">
                    +
                </button>
            </div>
        </div>
    );
}
