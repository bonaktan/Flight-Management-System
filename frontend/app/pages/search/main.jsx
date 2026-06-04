// responsibiltity: fetching flight data from backend and displaying it in a list of cards

import { useEffect, useState, use } from "react";
import { NavLink, useLoaderData } from "react-router";
import axios from "axios";
import { SearchParametersContext } from "./searchContext";

export default function Search() {
    const searchParams = use(SearchParametersContext);
    // todo: passengers are not yet handled

    if (searchParams.loading) {
        return <p>Loading...</p>;
    }
    return (
        <div>
            {searchParams.apiReturn ? (
                searchParams.apiReturn.map((flight) => <FlightCard key={flight.id} flight={flight} />)
            ) : searchParams.apiError ? (
                <p>Error occurred while fetching flight data. Error: {searchParams.apiError.error}</p>
            ) : (
                <p>No Flights are found.</p>
            )}
        </div>
    );
}

function FlightCard({ flight }) {
    console.log(flight.id);
    const [selectedClass, setSelectedClass] = useState();
    const [openSidebar, setOpenSidebar] = useState(false);
    return (
        <div className="flex gap-2 w-full">
            <div className="w-2/5 p-2 border border-[#ccc] rounded-sm flex flex-col gap-2">
                <p className="border-b-1">Flight ID</p>
                <div className="flex justify-center items-center">
                    <div id="origin" className="flex items-center gap-1 flex-1">
                        <p className="uppercase text-3xl">mnl</p>
                        <div className="flex flex-col gap-0">
                            <p className="leading-none text-sm">
                                Dec 31 <br />
                                19:31
                            </p>
                        </div>
                    </div>
                    <div className="w-full flex items-center">
                        <span className="material-symbols-outlined">flight_takeoff</span>
                        <div className="flex-1 w-full border-t-2 border-dotted border-altitude-tint" />
                        <span className="material-symbols-outlined">flight_land</span>
                    </div>
                    <div id="destination" className="flex items-center gap-1">
                        <div className="flex flex-col gap-0">
                            <p className="leading-none text-xs ">
                                Jan 5 <br />
                                20:00
                            </p>
                        </div>
                        <p className="uppercase text-3xl">ceb</p>
                    </div>
                </div>
            </div>

            {["P1,500", "P2,000", "P2,500"].map((price, key) => (
                <button
                    onClick={() => {
                        setSelectedClass(price);
                        console.log(price);
                    }}
                    key={key}
                    className={`transition border boorder-[#ccc] w-1/5 ${selectedClass == price ? "bg-blaze-core" : ""}`}>
                    {price}
                </button>
            ))}

            {/* <button className="border border-[#ccc] w-1/5">P1,500</button>
            <button className="border border-[#ccc] w-1/5">P2,000</button>
            <button className="border border-[#ccc] w-1/5">P2,500</button> */}
        </div>
    );
}
