import { NavLink } from "react-router";
import InputField from "../components/input";
import { useState } from "react";

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
            description:
                "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aliquam non enim dignissim leo commodo iaculis vitae eget erat. Nullam dignissim volutpat vestibulum. Quisque dignissim ligula eget velit lacinia tincidunt. Nam blandit diam eget lectus convallis, sit amet tristique lorem tincidunt. Nullam dapibus fringilla laoreet. Vivamus luctus id turpis in rutrum. In at cursus turpis. Cras accumsan, urna non porta rutrum, magna est porta quam, at suscipit enim felis dignissim lorem. Curabitur eu maximus diam. Proin nec mauris pulvinar, egestas augue non, vehicula neque. Maecenas at libero ligula. ",
            image: "/pubs/sample1.png",
        },
        {
            name: "Cebu",
            description: "Lorem ipsum dolor sit amet",
            image: "/pubs/sample2.png",
        },
        {
            name: "Boracay",
            description: "Lorem ipsum dolor sit amet",
            image: "/pubs/sample3.png",
        },
        {
            name: "New York",
            description: "Lorem ipsum dolor sit amet",
            image: "/pubs/sample4.png",
        },
        {
            name: "Seoul",
            description: "Lorem ipsum dolor sit amet",
            image: "/pubs/sample5.png",
        },
        {
            name: "Tokyo",
            description: "Lorem ipsum dolor sit amet",
            image: "/pubs/sample6.png",
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
            <HomepageCard places={apiOutput.fly_to} />
        </div>
    );
}

function Hero() {
    return (
        <div className="flex-1 flex flex-col items-center justify-center">
            <p className="lg:text-5xl text-3xl">SkyBridge Airways</p>
            <p className="lg:text-3xl text-xl">ang tagline na hindi matino!</p>
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

function HomepageCard({ places }) {
    const [selectedPlace, setSelectedPlace] = useState(0);
    return (
        <div className="relative h-[75vh]">
            <div className="relative h-full w-full">
                <button
                    className="absolute right-0 top-0 flex h-full items-center justify-center px-2 hover:bg-gray-100"
                    disabled={selectedPlace >= places.length - 1}
                    onClick={() => {
                        setSelectedPlace(selectedPlace + 1);
                        console.log(selectedPlace);
                    }}>
                    <span className="rotate-90 transform whitespace-nowrap text-sm">next</span>
                </button>

                <div style={{ backgroundImage: `url(${places[selectedPlace].image})` }} className="p-6 h-full w-full"></div>
            </div>
            <div className="absolute top-0 left-0 h-full w-1/2 bg-linear-to-r from-white from-50% to-transparent">
                <button
                    className="absolute left-0 top-0 flex h-full items-center justify-center px-2 hover:bg-gray-100"
                    disabled={selectedPlace <= 0}
                    onClick={() => setSelectedPlace(selectedPlace - 1)}>
                    <span className="rotate-90 transform whitespace-nowrap text-sm">prev</span>
                </button>

                <div className="p-6">
                    <h1 className="text-2xl font-bold">{places[selectedPlace].name}</h1>
                    <p className="mt-2 w-1/2">{places[selectedPlace].description}</p>
                </div>
            </div>
        </div>
    );
}
