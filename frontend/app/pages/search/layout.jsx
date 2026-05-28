// responsibiltity: layout for search page, will contain search bar and filter controls
// also responsible for rendering the search results, which will be fetched in the main.jsx file and passed down as props to the search result component
import InputField from "../../components/input";
import { Outlet } from "react-router";
export default function SearchLayout() {
    return (
        <div>
            <div id="search-bar" className="flex flex-row p-5 align-bottom">
                <InputField name="Origin" />
                <InputField name="Destination" />
                <InputField name="Departure Date" />
                <InputField name="Passengers" />
                <button className="border p-2 h-10 align-baseline">Search</button>
            </div>
            <div id="filters" className="flex flex-row align-bottom px-5 gap-5">
                <div>Sort</div>
                <div>Price</div>
                <div>Flight Duration</div>
                <div>Departure Date</div>
            </div>
            <Outlet />
        </div>
    );
}
