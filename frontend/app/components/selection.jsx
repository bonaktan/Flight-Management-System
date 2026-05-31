import "./stylesheets/selection.css";

export default function SelectionField({ name, children, selDesign, labDesign, ...props }) {
    return (
        <div className="w-full">
            <label className={labDesign}>{name}</label>
            <select id="" className={`h-10 border border-black rounded-sm w-full ${selDesign}`} {...props}>
                {children}
            </select>
        </div>
    );
}
