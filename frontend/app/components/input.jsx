export default function InputField({ name, icon, ...props }) {
    return (
        <div className="flex flex-col w-full">
            <label className="">{name}</label>
            <div className="h-10 p-1 border flex gap-1 items-center rounded-sm w-full">
                <p className="material-symbols-outlined select-none">{icon}</p>
                <input className="w-full focus:outline-none" {...props} />
            </div>
        </div>
    );
}
