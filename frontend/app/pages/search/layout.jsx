import { Outlet, NavLink } from "react-router";
export default function SearchLayout() {
    return (
        <div>
            <p>Search - Layout</p>
            <div>
                <Outlet />
            </div>
        </div>
    );
}
