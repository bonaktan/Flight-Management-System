import { Form, NavLink } from "react-router";
import { useState } from "react";
import { InputField, SelectionField } from "../components/input";
import hero from "../../public/hero.jpg";
import "./stylesheets/home.css";
import manila from "../assets/pubs/manila.jpg";
import cebu from "../assets/pubs/cebu.jpg";
import boracay from "../assets/pubs/boracay.jpg";
import newYork from "../assets/pubs/new york.jpg";
import seoul from "../assets/pubs/seoul.jpg";
import tokyo from "../assets/pubs/tokyo.jpg";
import axios from "axios";

const apiUrl = import.meta.env.VITE_BACKEND_URL;

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
            description: "Lorem ipsum dolor sit amet.",
            image: manila,
        },
        {
            name: "Cebu",
            description: "Lorem ipsum dolor sit amet",
            image: cebu,
        },
        {
            name: "Boracay",
            description: "Lorem ipsum dolor sit amet",
            image: boracay,
        },
        {
            name: "New York",
            description: "Lorem ipsum dolor sit amet",
            image: newYork,
        },
        {
            name: "Seoul",
            description: "Lorem ipsum dolor sit amet",
            image: seoul,
        },
        {
            name: "Tokyo",
            description: "Lorem ipsum dolor sit amet",
            image: tokyo,
        },
    ],
};
export async function loader() {
    const airports = (await axios.get(`${apiUrl}/api/search/airports`)).data;
    const date = Date.now();
    return { airports: airports, date: date };
}
export default function Home({ loaderData }) {
    return (
        <div>
            <div id="hero" className={`h-[75dvh] flex flex-col`}>
                {/* <div className="w-full h-full bg-wine-core opacity-60 absolute z-2"/> */}
                <Hero />
                <Search data={loaderData} />
            </div>
            <HomepageCard places={apiOutput.fly_to} />
        </div>
    );
}

// Photo by <a href="https://unsplash.com/@brokk?utm_source=unsplash&utm_medium=referral&utm_content=creditCopyText">alexey starki</a> on <a href="https://unsplash.com/photos/white-airplane-under-blue-sky-during-daytime-91ykdj2WQeg?utm_source=unsplash&utm_medium=referral&utm_content=creditCopyText">Unsplash</a>

function Hero() {
    return (
        <div className="flex-1 flex flex-col items-center justify-center text-cloud-warm z-4">
            <p className="lg:text-5xl text-3xl tracking-wider">SkyBridge Airways</p>
            <p className="lg:text-3xl text-xl">Bridging the skies.</p>
        </div>
    );
}

function Search({ data }) {
    const [selectedMode, setSelectedMode] = useState(0);
    return (
        <div id="searchbar" className="flex justify-center items-center flex-col gap-2 mx-2 mb-2 z-4">
            <p className="font-medium md:text-4xl text-cloud-warm">Book a Flight</p>
            <div className="flex gap-2 shadow-md border lg:w-9/12 w-full flex-col bg-cloud-warm border-gray-200 rounded-md p-4">
                <div id="route_select" className="rounded-sm flex h-10  align-center">
                    {Object.entries(apiOutput.modes).map(([key, value], i) => (
                        <button
                            onClick={() => setSelectedMode(i)}
                            key={key}
                            className={`transition flex justify-center items-center w-32 text-center cursor-pointer ${selectedMode == i ? "bg-blaze-core text-blaze-tint" : ""}`}>
                            {value.name}
                        </button>
                    ))}
                </div>
                <div id="input_fields" className="flex w-full gap-1 lg:flex-row flex-col justify-center">
                    <div id="places" className="flex w-full gap-1 lg:flex-rowflex-col">
                        <SelectionField name="Origin" labDesign={`text-blaze-deep`} selDesign="bg-blaze-tint text-altitude-ink" defaultValue="">
                            <option value="" disabled>
                                -Select Origin-
                            </option>
                            {data.airports.map((airport) => (
                                <option key={airport.id} value={airport.id}>
                                    {airport.id} - {airport.name}, {airport.place}
                                </option>
                            ))}
                        </SelectionField>
                        <SelectionField name="Destination" labDesign={`text-blaze-deep`} selDesign="bg-blaze-tint text-altitude-ink" defaultValue="">
                            <option value="" disabled>
                                -Select Destination-
                            </option>
                            {data.airports.map((airport) => (
                                <option key={airport.id} value={airport.id}>
                                    {airport.id} - {airport.name}, {airport.place}
                                </option>
                            ))}
                        </SelectionField>
                        <InputField type="date" name="Departure Date" inDesign={`bg-blaze-tint`} labDesign={`text-blaze-deep`} min={data.date} />
                        {selectedMode == 1 ? (
                            <InputField type="date" name="Return Date" inDesign={`bg-blaze-tint`} labDesign={`text-blaze-deep`} min={data.date} />
                        ) : (
                            <></>
                        )}
                    </div>
                </div>
                <div className="flex justify-center lg:h-10 lg:self-end lg:mt-auto mt-5">
                    <NavLink to="/search" className="border flex justify-center items-center w-75 rounded-sm ">
                        Search
                    </NavLink>
                </div>
            </div>
        </div>
    );
}

function HomepageCard({ places }) {
    const [selectedPlace, setSelectedPlace] = useState(0);
    return (
        <div className="relative h-[75vh]">
            <div className="relative h-full w-full">
                <img
                    src={places[selectedPlace].image}
                    alt={places[selectedPlace].name}
                    className="h-full w-full object-cover lg:object-right object-center"
                />
            </div>
            <div className="absolute top-0 left-0 lg:h-full lg:w-full w-full h-1/2 lg:bg-linear-to-r bg-linear-to-b from-cloud-warm lg:from-30% from-80% to-transparent">
                <div className="ml-10 p-6">
                    <h1 className="text-2xl font-bold text-cloud-pop">{places[selectedPlace].name}</h1>
                    <p className="mt-2 lg:w-1/2 lg:h-auto h-1/2 text-altitude-ink">{places[selectedPlace].description}</p>
                </div>
            </div>
            <button
                className="absolute right-0 top-0 flex h-full items-center justify-center px-2 text-altitude-ink hover:text-blaze-core cursor-pointer"
                disabled={selectedPlace >= places.length - 1}
                onClick={() => {
                    setSelectedPlace(selectedPlace + 1);
                    console.log(selectedPlace);
                }}>
                <span className="material-symbols-outlined">chevron_right</span>
            </button>
            <button
                className="absolute left-0 top-0 flex h-full items-center justify-center px-2 text-altitude-ink hover:text-blaze-core cursor-pointer"
                disabled={selectedPlace <= 0}
                onClick={() => setSelectedPlace(selectedPlace - 1)}>
                <span className="material-symbols-outlined">chevron_left</span>
            </button>
        </div>
    );
}
