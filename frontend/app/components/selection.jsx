export default function SelectionField({ name, children }) {
    return (
        <div className="w-full">
            <label>{name}</label>
            <select className="h-10 border rounded-sm w-full">{children}</select>
        </div>
    );
}
