export default function InputField({ name, icon, ...props }) {
    return (
        <div className="flex flex-col w-full">
            <label className="text-base">{name}</label>
            <div className="flex-1 h-10 p-1 border flex gap-1 items-center">
                <p className="material-symbols-outlined select-none">{icon}</p>
                <input className="flex-1 focus:outline-none" {...props} />
            </div>
        </div>
    );
}
