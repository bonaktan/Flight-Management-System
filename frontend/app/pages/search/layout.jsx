// responsibiltity: layout for search page, will contain search bar and filter controls
// also responsible for rendering the search results, which will be fetched in the main.jsx file and passed down as props to the search result component
import { InputField } from "../../components/input";
import Bookpop from "./priceComponent";
import { Outlet, useSearchParams } from "react-router";
import { SearchParametersContext } from "./searchContext";
import { useState, useReducer } from "react";
import axios from "axios";
const apiUrl = import.meta.env.VITE_BACKEND_URL;

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
            return_date: searchParams.get("return_date") || "",
            sort: searchParams.get("sort") || "price",
            selectedFlight: null,
            selectedClass: null,
            passengers: searchParams.get("passengers") || 1,
        },
    );
    function fetchData() {
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

    const [debug, setDebug] = useState(false);
    return (
        <SearchParametersContext value={{ ...searchContext, setSearchContext: setSearchContext }}>
            <div id="search-bar" className="flex flex-row p-5 align-bottom">
                <InputField
                    label="Origin"
                    defaultValue={searchContext["origin"]}
                    onBlur={(e) => setSearchContext({ field: "origin", value: e.target.value })}
                    onKeyDown={(e) => {
                        if (event.key === "Enter") {
                            setSearchContext({ field: "origin", value: e.target.value });
                        }
                    }}
                />
                <InputField
                    label="Destination"
                    defaultValue={searchContext["destination"]}
                    onBlur={(e) => setSearchContext({ field: "destination", value: e.target.value })}
                    onKeyDown={(e) => {
                        if (event.key === "Enter") {
                            setSearchContext({ field: "destination", value: e.target.value });
                        }
                    }}
                />
                <InputField
                    label="Departure Date"
                    defaultValue={searchContext["departure_date"]}
                    onBlur={(e) => setSearchContext({ field: "departure_date", value: e.target.value })}
                    onKeyDown={(e) => {
                        if (event.key === "Enter") {
                            setSearchContext({ field: "departure_date", value: e.target.value });
                        }
                    }}
                />
                <InputField
                    label="Return Date"
                    defaultValue={searchContext["return_date"]}
                    onBlur={(e) => setSearchContext({ field: "return_date", value: e.target.value })}
                    onKeyDown={(e) => {
                        if (event.key === "Enter") {
                            setSearchContext({ field: "return_date", value: e.target.value });
                        }
                    }}
                />
                <button onClick={fetchData} className="border p-2 h-10 align-baseline">
                    Search
                </button>
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
