import { NavLink } from "react-router";
import InputField from "../components/input";

const apiOutput = {
    modes: {
        one_way: {
            name: "One Way",
        },
        round_trip: {
            name: "Round Trip",
        },
    },
    fly_to: [
        {
            name: "Manila",
            description: "Lorem ipsum dolor sit amet",
        },
        {
            name: "Cebu",
            description: "Lorem ipsum dolor sit amet",
        },
        {
            name: "Boracay",
            description: "Lorem ipsum dolor sit amet",
        },
        {
            name: "New York",
            description: "Lorem ipsum dolor sit amet",
        },
        {
            name: "Seoul",
            description: "Lorem ipsum dolor sit amet",
        },
        {
            name: "Tokyo",
            description: "Lorem ipsum dolor sit amet",
        },
    ],
};

export default function Home() {
    return (
        <div>
            <div id="hero" className="h-[75dvh] flex flex-col">
                <Hero />
                <Search />
            </div>
            <div id="misc">
                <p className="text-center">Visit now!</p>
                <div className="grid grid-cols-2 lg:grid-cols-3 lg:gap-10 gap-5 m-10 place-items-center-safe">
                    {apiOutput.fly_to.map((destination) => (
                        <HomepageCard key={destination.name} destination={destination} />
                    ))}
                </div>
            </div>
        </div>
    );
}

function Hero() {
    return (
        <div className="flex-1 flex flex-col items-center justify-center">
            <p class="lg:text-5xl text-3xl">SkyBridge Airways</p>
            <p class="lg:text-3xl text-xl">ang tagline na hindi matino!</p>
        </div>
    );
}

function Search() {
    return (
        <div id="searchbar" className="flex flex-col flex-1 align-center justify-center">
            <p className="text-center">Book a Flight</p>
            <div id="route_select" className="flex gap-2 justify-center ">
                {Object.entries(apiOutput.modes).map(([key, value]) => (
                    <div key={key} className="border p-2 w-32 text-center">
                        {value.name}
                    </div>
                ))}
            </div>
            <div id="input_fields" className="flex lg:flex-row flex-col gap-4 justify-center m-10 mb-2">
                <div id="places" className="flex lg:flex-row flex-col flex-1 justify-center">
                    <InputField name="Origin" icon="flight_takeoff" />
                    <InputField name="Destination" icon="flight_land" />
                </div>
                <div id="dates" className="flex lg:flex-row flex-col flex-1 justify-center">
                    <InputField name="Departure Date" icon="calendar_month" />
                    <InputField name="Return Date" icon="calendar_month" />
                </div>
            </div>
            <div className="m-10 mt-1 flex justify-center">
                <NavLink to="/search" className="border p-2 flex-1 text-center">
                    Search for Flights
                </NavLink>
            </div>
        </div>
    );
}

function HomepageCard({ destination }) {
    return (
        <div className="bg-white rounded-xl overflow-hidden shadow-md w-full max-w-lg">
            <div className="lg:aspect-video aspect-square bg-blue-400"></div>
            <div className="p-6">
                <p className="lg:text-xl text-lg mb-2">{destination.name}</p>
                <p className="lg:text-lg text-sm">{destination.description}</p>
            </div>
        </div>
    );
}
