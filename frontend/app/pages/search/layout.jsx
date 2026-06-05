// responsibiltity: layout for search page, will contain search bar and filter controls
// also responsible for rendering the search results, which will be fetched in the main.jsx file and passed down as props to the search result component
import InputField from "../../components/input";
import Bookpop from "./priceComponent";
import { Outlet, useSearchParams } from "react-router";
import { SearchParametersContext } from "./searchContext";
import { useState, useReducer } from "react";
import axios from "axios";
const apiUrl = import.meta.env.VITE_BACKEND_URL;

function SearchInput({ name, type }) {
    return (
        <div className="flex flex-col w-full">
            <label for={name} className="text-sm">
                {name}
            </label>
            <input name={name} type={type} className="border rounded-sm px-2" />
        </div>
    );
}

export default function SearchLayout() {
    const [searchParams] = useSearchParams();
    const [searchContext, setSearchContext] = useReducer(
        (state, action) => {
            return { ...state, [action.field]: action.value };
        },
        {
            apiReturn: null,
            apiError: null,
            loading: true,
            origin: searchParams.get("origin") || "",
            destination: searchParams.get("destination") || "",
            departure_date: searchParams.get("departure_date") || "",
            sort: searchParams.get("sort") || "price",
        },
    );
    function fetchData() {
        // setSearchContext({ field: "loading", value: true });
        // setSearchContext({ field: "apiReturn", value: null });
        // setSearchContext({ field: "apiError", value: null });
        axios
            .get(`${apiUrl}/api/search/flights`, {
                params: {
                    origin: searchContext["origin"],
                    destination: searchContext["destination"],
                    departure_date: searchContext["departure_date"],
                    timezone: Intl.DateTimeFormat().resolvedOptions().timeZone,
                    passengers: 1,
                    sort_by: searchContext["sort"],
                },
            })
            .then((ret) => {
                setSearchContext({ field: "apiReturn", value: ret.data });
                console.log(ret);
            })
            .catch((err) => {
                setSearchContext({ field: "apiError", value: err.response.data });
                setSearchContext({ field: "apiReturn", value: null });
                console.log(err.response.data);
            })
            .finally(async () => {
                setSearchContext({ field: "loading", value: false });
            });
    }
    if (searchContext.apiReturn === null && searchContext.apiError === null) {
        fetchData();
    }
    // BUG: when the user changes the search parameters, the search results are not updated until the user clicks the search button again. This is because the fetchData function is only called when the component is first rendered, and not
    const [selectedMode, setSelectedMode] = useState();
    const [debug, setDebug] = useState(false);
    return (
        <SearchParametersContext value={searchContext}>
            <div id="search-bar" className="flex flex-row gap-2 p-2 align-bottom border my-2 mx-4 shadow-xl bg-blaze-tint">
                <div className="flex items-center">
                    {["Oneway", "Roundtrip"].map((trip, _) => (
                        <button
                            onClick={() => setSelectedMode(trip)}
                            className={`${selectedMode == trip ? "bg-blaze-core text-white" : ""} border p-2 transition`}>
                            {trip}
                        </button>
                    ))}
                </div>
                <div className="m-0 p-0 flex items-center gap-2 w-full">
                    <SearchInput name="Origin" />
                    <span className="flex w-12 overflow-hidden translate-y-1 ">
                        <span className={`material-symbols-outlined ${selectedMode == "Roundtrip" ? "" : "-translate-x-6 rotate-90"} transition`}>
                            compare_arrows
                        </span>
                        <span className={`material-symbols-outlined  ${selectedMode == "Roundtrip" ? "" : "-translate-x-6 rotate-90"} transition`}>
                            flight
                        </span>
                    </span>
                    <SearchInput name="Destination" />
                </div>
                <div className="flex gap-2">
                    <SearchInput name="Date" type="date" />
                    <SearchInput name="Return Date" type="date" />
                </div>
                <button className="w-full">Submit</button>
            </div>
            <div id="filters" className="flex flex-row align-bottom px-5 gap-5">
                <div>Sort</div>
                <button onClick={() => setSearchContext({ ...searchContext, sort: "price" })}>Price</button>
                <button onClick={() => setSearchContext({ ...searchContext, sort: "flight_time" })}>Flight Duration</button>
                <button onClick={() => setSearchContext({ ...searchContext, sort: "departure" })}>Departure Time</button>
            </div>
            <div id="results" className="flex flex-col gap-2 p-5">
                <div id="header" className="flex w-full gap-2">
                    <div className="w-2/5" />
                    <div className="text-center w-1/5">Essentials</div>
                    <div className="text-center w-1/5">Popular</div>
                    <div className="text-center w-1/5">Ultimate</div>
                </div>
                <Outlet />
                <input
                    onClick={() => {
                        setDebug(!debug);
                        console.log(debug);
                    }}
                    type="checkbox"
                />
                <Bookpop activity={debug} />
            </div>
        </SearchParametersContext>
    );
}
