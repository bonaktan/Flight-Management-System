import { NavLink } from "react-router";

export default function Search() {
    return (
        <>
            <p>Search View - Main</p>
            {Object.entries({ PH6769: {} }).map(([flightID, data]) => (
                <div key={flightID} className="flex justify-between">
                    <div>
                        <p>Flight {flightID}</p>
                        <NavLink to="/search/details">Details</NavLink>
                    </div>
                    <NavLink to="/booking/form">Book</NavLink>
                </div>
            ))}
        </>
    );
}
