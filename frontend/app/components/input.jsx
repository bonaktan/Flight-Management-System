import "./stylesheets/selection.css";

export function InputField({ name, icon, genDesign, inDesign, labDesign, ...props }) {
    return (
        <div className={`flex flex-col w-full ${genDesign}`}>
            <label className={`${labDesign}`}>{name}</label>
            <div className={`h-10 p-1 border flex gap-1 items-center rounded-sm w-full ${inDesign}`}>
                <p className="material-symbols-outlined select-none">{icon}</p>
                <input className={`w-full focus:outline-none ${inDesign}`} {...props} />
            </div>
        </div>
    );
}

export function SelectionField({ name, children, selDesign, labDesign, ...props }) {
    return (
        <div className="w-full">
            <label className={labDesign}>{name}</label>
            <select id="" className={`h-10 border border-black rounded-sm w-full ${selDesign}`} {...props}>
                {children}
            </select>
        </div>
    );
}
