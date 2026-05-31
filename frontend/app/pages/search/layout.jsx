// responsibiltity: layout for search page, will contain search bar and filter controls
// also responsible for rendering the search results, which will be fetched in the main.jsx file and passed down as props to the search result component
import InputField from "../../components/input";
import { Outlet, useSearchParams } from "react-router";
import { SearchParametersContext } from "./searchContext";
export default function SearchLayout() {
    const [searchParams] = useSearchParams();

    return (
        <SearchParametersContext value={searchParams}>
            <div id="search-bar" className="flex flex-row p-5 align-bottom">
                <InputField label="Origin" defaultValue={searchParams.get("origin")} />
                <InputField label="Destination" defaultValue={searchParams.get("destination")} />
                <InputField label="Departure Date" defaultValue={searchParams.get("departure_date")} />
                <InputField label="Passengers" defaultValue="1  " />
                <button className="border p-2 h-10 align-baseline">Search</button>
            </div>
            <div id="filters" className="flex flex-row align-bottom px-5 gap-5">
                <div>Sort</div>
                <div>Price</div>
                <div>Flight Duration</div>
                <div>Departure Date</div>
            </div>
            <Outlet />
        </SearchParametersContext>
    );
}
