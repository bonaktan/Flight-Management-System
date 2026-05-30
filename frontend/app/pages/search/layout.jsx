// responsibiltity: layout for search page, will contain search bar and filter controls
// also responsible for rendering the search results, which will be fetched in the main.jsx file and passed down as props to the search result component
import InputField from "../../components/input";
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
    return (
        <SearchParametersContext value={searchContext}>
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
                    label="Passengers"
                    defaultValue="1"
                    onBlur={(e) => setSearchContext({ field: "passengers", value: e.target.value })}
                    onKeyDown={(e) => {
                        if (event.key === "Enter") {
                            setSearchContext({ field: "passengers", value: e.target.value });
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
            <Outlet />
        </SearchParametersContext>
    );
}
