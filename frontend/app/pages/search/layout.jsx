// responsibiltity: layout for search page, will contain search bar and filter controls
// also responsible for rendering the search results, which will be fetched in the main.jsx file and passed down as props to the search result component
import { CounterField, InputField, SelectionField } from "../../components/input";
import { createSearchParams, Outlet, useNavigate } from "react-router";
import { SearchParametersContext } from "./searchContext";
import { use, useReducer } from "react";
import axios from "axios";
const apiUrl = import.meta.env.VITE_BACKEND_URL;

function Bookpop() {
    const searchParams = use(SearchParametersContext);
    console.log("searchContext in Bookpop: ", searchParams);
    const navigate = useNavigate();
    function onConfirm() {
        navigate("/booking/form", {
            state: {
                flightId: searchParams.selectedFlightAndClass.flight,
                class: searchParams.selectedFlightAndClass.seatClass,
                passengers: searchParams.passengers,
                departure_date: searchParams.departure_date,
            },
        });
    }
    return (
        <div
            className={`w-full border-2 p-2 flex justify-between transition ${searchParams.selectedFlightAndClass ? "active" : ""} absolute -bottom-20 `}>
            {searchParams.selectedFlightAndClass.flight}
            <div className="flex justify-start flex-col">
                <p>Flight ID</p>
                <p className="flex items-center gap-2">
                    MNL
                    <span className="material-symbols-outlined rotate-90">flight</span>
                    CEB
                </p>
            </div>
            <div className="text-center">
                <p className="font-bold">Passengers</p>
                <p>2</p>
            </div>
            <button className="border px-4" onClick={onConfirm}>
                Book
            </button>
        </div>
    );
}

async function fetchFlights(searchContext) {
    let apiReturn = { apiReturn: null, apiError: null };
    console.log("searchContext: ", searchContext);
    try {
        apiReturn.apiReturn = (
            await axios.get(`${apiUrl}/api/search/flights`, {
                params: {
                    origin: searchContext["origin"],
                    destination: searchContext["destination"],
                    departure_date: searchContext["departure_date"],
                    timezone: Intl.DateTimeFormat().resolvedOptions().timeZone,
                    passengers: searchContext["passengers"],
                    sort_by: searchContext["sort"],
                },
            })
        ).data;
    } catch (err) {
        apiReturn.apiError = err.response?.data || { error: err.message || "An unknown error occurred" };
    }
    return apiReturn;
}

export async function loader(request) {
    const searchParams = new URL(request.url).searchParams;
    console.log(searchParams);
    const airports = (await axios.get(`${apiUrl}/api/search/airports`)).data;
    return { airports: airports, searchParams: Object.fromEntries(searchParams) };
}
export async function clientLoader({ serverLoader }) {
    const serverData = await serverLoader();
    const flights = await fetchFlights(serverData.searchParams);
    return { flights: flights, ...serverData };
}
clientLoader.hydrate = true;

export default function SearchLayout({ loaderData }) {
    const navigate = useNavigate();
    const [selectedFlightAndClass, setSelectedFlightAndClass] = useReducer(
        (state, { flight, seatClass }) => {
            let ret;
            if (flight == state.flight && seatClass == state.seatClass) ret = { flight: null, seatClass: null };
            else ret = { flight: flight, seatClass: seatClass };
            return ret;
        },
        { flight: null, seatClass: null },
    );
    const [searchContext, setSearchContext] = useReducer(
        (state, action) => {
            const ret = { ...state };
            if (action.field == "origin" && action.value == ret.destination) {
                ret.destination = ret.origin;
                ret.origin = action.value;
            } else if (action.field == "destination" && action.value == state.origin) {
                ret.origin = ret.destination;
                ret.destination = action.value;
            } else {
                ret[action.field] = action.value;
            }
            return ret;
        },
        {
            flight_mode: loaderData.searchParams["return_date"] ? "Round Trip" : "One Way",
            origin: loaderData.searchParams["origin"] || "",
            destination: loaderData.searchParams["destination"] || "",
            departure_date: loaderData.searchParams["departure_date"] || "",
            return_date: loaderData.searchParams["return_date"] || "",
            passengers: loaderData.searchParams["passengers"] || 1,
            sort: loaderData.searchParams["sort"] || "price",
        },
    );

    function searchUpdate() {
        navigate({
            pathname: "/search",
            search: `?${createSearchParams({
                origin: searchContext["origin"],
                destination: searchContext["destination"],
                departure_date: searchContext["departure_date"],
                passengers: searchContext["passengers"],
                ...(searchContext.flight_mode == "Round Trip" ? { return_date: searchContext["return_date"] } : {}),
            })}`,
        });
    }
    return (
        <SearchParametersContext
            value={{
                ...loaderData.flights,
                ...searchContext,
                selectedFlightAndClass: selectedFlightAndClass,
                setSelectedFlightAndClass: setSelectedFlightAndClass,
            }}>
            <div id="search-bar" className="flex flex-row gap-2 p-2 align-bottom border my-2 mx-4 shadow-xl bg-blaze-tint">
                <div className="flex flex-col">
                    <label className="text-sm text-center">Flight Mode</label>
                    <div className="flex items-center w-fit">
                        {["One Way", "Round Trip"].map((trip) => (
                            <button
                                onClick={() => setSearchContext({ field: "flight_mode", value: trip })}
                                key={trip}
                                className={`${searchContext.flight_mode == trip ? "bg-blaze-core text-white" : ""} border px-2 text-sm transition whitespace-nowrap h-10`}>
                                {trip}
                            </button>
                        ))}
                    </div>
                </div>

                <div className="m-0 p-0 flex items-center gap-2 w-full">
                    <SelectionField
                        name="Origin"
                        genDesign="w-full"
                        labDesign="text-sm"
                        selDesign="rounded-sm text-sm"
                        onChange={(e) => {
                            console.log("select: ", e.target.value);
                            setSearchContext({ field: "origin", value: e.target.value });
                        }}
                        value={searchContext.origin}
                        required>
                        {loaderData.airports.map((airport) => (
                            <option key={airport.id} value={airport.id}>
                                {airport.id} - {airport.name}
                            </option>
                        ))}
                    </SelectionField>
                    <span className="flex w-12 overflow-hidden translate-y-1 ">
                        <span
                            className={`material-symbols-outlined ${searchContext.flight_mode == "Round Trip" ? "" : "-translate-x-6 rotate-90"} transition`}>
                            compare_arrows
                        </span>
                        <span
                            className={`material-symbols-outlined  ${searchContext.flight_mode == "Round Trip" ? "" : "-translate-x-6 rotate-90"} transition`}>
                            flight
                        </span>
                    </span>
                    <SelectionField
                        name="Destination"
                        genDesign="w-full"
                        labDesign="text-sm"
                        selDesign="rounded-sm text-sm"
                        onChange={(e) => setSearchContext({ field: "destination", value: e.target.value })}
                        value={searchContext.destination}
                        required>
                        {loaderData.airports.map((airport) => (
                            <option key={airport.id} value={airport.id}>
                                {airport.id} - {airport.name}
                            </option>
                        ))}
                    </SelectionField>
                </div>
                <div className="flex gap-2 w-110">
                    <InputField
                        name="Date"
                        type="date"
                        genDesign="w-full"
                        labDesign="text-sm"
                        inDesign="rounded-sm text-sm"
                        onChange={(e) => setSearchContext({ field: "departure_date", value: e.target.value })}
                        value={searchContext.departure_date}
                    />
                    {searchContext.flight_mode == "Round Trip" ? (
                        <InputField
                            name="Return Date"
                            type="date"
                            genDesign="w-full"
                            labDesign="text-sm"
                            inDesign="rounded-sm text-sm"
                            onChange={(e) => setSearchContext({ field: "return_date", value: e.target.value })}
                            value={searchContext.return_date}
                        />
                    ) : (
                        <></>
                    )}
                </div>
                <CounterField
                    name="Passengers"
                    genDesign="w-fit"
                    labDesign="text-sm"
                    min={1}
                    max={9}
                    value={searchContext.passengers}
                    onChange={(num) => setSearchContext({ field: "passengers", value: num })}
                />
                <button className="w-64" onClick={searchUpdate}>
                    Submit
                </button>
            </div>
            <div id="filters" className="flex flex-row align-bottom px-5 gap-5">
                <div>Sort</div>
                <button onClick={() => setSearchContext({ field: "sort", value: "price" })}>Price</button>
                <button onClick={() => setSearchContext({ field: "sort", value: "flight_time" })}>Flight Duration</button>
                <button onClick={() => setSearchContext({ field: "sort", value: "departure" })}>Departure Time</button>
            </div>
            <div id="results" className="flex flex-col gap-2 p-5">
                <div id="header" className="flex w-full gap-2">
                    <div className="w-2/5" />
                    <div className="text-center w-1/5">Essentials</div>
                    <div className="text-center w-1/5">Popular</div>
                    <div className="text-center w-1/5">Ultimate</div>
                </div>
                <Outlet />
            </div>
            <Bookpop />
        </SearchParametersContext>
    );
}
