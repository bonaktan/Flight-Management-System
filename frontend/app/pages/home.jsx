import { createSearchParams, Form, useNavigate } from "react-router";
import { useState } from "react";
import { CounterField, InputField, SelectionField } from "../components/input";
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

export function meta() {
    return [{ title: "SkyBridge Airlines" }];
}

export async function loader() {
    const airports = (await axios.get(`${apiUrl}/api/search/airports`)).data;
    const date = new Date().toISOString().split("T")[0];
    return {
        airports: airports,
        date: date,
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
                    "Where the city never quite sleeps — and somehow, neither do you. Loud, warm, and alive in ways that take a few days to understand.",
                image: manila,
            },
            {
                name: "Cebu",
                description:
                    "A city that earns its slowness. Between the sea and the hills, time moves differently here — and that's exactly the point.",
                image: cebu,
            },
            {
                name: "Boracay",
                description: "White sand fine enough to forget everything. Stay long enough and you might not remember why you were in a hurry.",
                image: boracay,
            },
            {
                name: "New York",
                description:
                    "The city that never sleeps. A place where dreams are made, and sometimes broken, but always chased with relentless energy.",
                image: newYork,
            },
            {
                name: "Seoul",
                description: "The capital of South Korea, known for its vibrant culture, modern architecture, and delicious cuisine.",
                image: seoul,
            },
            {
                name: "Tokyo",
                description: "The bustling capital of Japan, where tradition meets innovation in a way that's both exciting and overwhelming.",
                image: tokyo,
            },
        ],
    };
}
export async function clientLoader({ serverLoader }) {
    return serverLoader();
}
clientLoader.hydrate = true; // show HydrateFallback on initial load
export function HydrateFallback() {
    return (
        <div className="flex justify-center items-center">
            <p>Loading...</p>
        </div>
    );
}
export default function Home({ loaderData }) {
    return (
        <div>
            <div id="hero" className={`h-[75dvh] flex flex-col`}>
                {/* <div className="w-full h-full bg-wine-core opacity-60 absolute z-2"/> */}
                <Hero />
                <Search data={loaderData} />
            </div>
            <HomepageCard places={loaderData.fly_to} />
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
    const navigate = useNavigate();
    const [flightMode, setFlightMode] = useState(0);
    const [origin, setOrigin] = useState("");
    const [destination, setDestination] = useState("");
    const [passengers, setPassengers] = useState(1);
    const [error, setError] = useState("");

    function submitSearch(e) {
        e.preventDefault();
        let form = new FormData(e.target);
        for (const [_key, value] of form.entries()) {
            if (!value || (typeof value === "string" && value.trim() === "")) {
                setError(`Fill out the ${_key} Field.`);
                return false;
            }
        }

        // 1 = roundtrip
        if (flightMode == 1) {
            const departure = new Date(form.get("Departure Date"));
            const arrival = new Date(form.get("Return Date"));
            if (departure >= arrival) {
                alert(`Return cannot be sooner than Departure`);
                return false;
            }
        }

        navigate({
            pathname: "/search",
            search: `?${createSearchParams({
                origin: form.get("Origin"),
                destination: form.get("Destination"),
                departure_date: form.get("Departure Date"),
                passengers: passengers,
                ...(flightMode == 1 ? { return_date: form.get("Return Date") } : {}),
            })}`,
        });
    }

    return (
        <div id="searchbar" className="flex justify-center items-center flex-col gap-2 mx-2 mb-2 z-4">
            <p className="font-medium md:text-4xl text-cloud-warm">Book a Flight</p>
            <Form
                className="flex gap-2 shadow-md border lg:w-9/12 w-full flex-col bg-cloud-warm border-gray-200 rounded-md p-4"
                onSubmit={submitSearch}>
                <div id="route_select" className="rounded-sm flex h-10  align-center">
                    {Object.entries(data.modes).map(([key, value], i) => (
                        <button
                            onClick={() => setFlightMode(i)}
                            type="button"
                            key={key}
                            value={key}
                            className={`transition flex justify-center items-center w-32 text-center cursor-pointer ${flightMode == i ? "bg-blaze-core text-blaze-tint" : ""} ${key == "round_trip" && "opacity-20 hover:cursor-not-allowed"}`}
                            disabled={key == "round_trip"}>
                            {value.name}
                        </button>
                    ))}
                </div>
                <div id="input_fields" className="flex w-full gap-1 lg:flex-row flex-col justify-center">
                    <div id="places" className="flex w-full gap-1 lg:flex-rowflex-col">
                        <SelectionField
                            name="Origin"
                            genDesign="w-full"
                            labDesign="text-blaze-deep"
                            selDesign="bg-blaze-tint text-altitude-ink"
                            defaultValue=""
                            onChange={(e) => setOrigin(e.target.value)}
                            value={origin}
                            required>
                            <option value="" disabled>
                                -Select Origin-
                            </option>
                            {data.airports.map((airport) => (
                                <option key={airport.id} value={airport.id} disabled={airport.id == destination}>
                                    {airport.id} - {airport.name}, {airport.place}
                                </option>
                            ))}
                        </SelectionField>
                        <SelectionField
                            name="Destination"
                            genDesign="w-full"
                            labDesign="text-blaze-deep"
                            selDesign="bg-blaze-tint text-altitude-ink"
                            defaultValue=""
                            onChange={(e) => setDestination(e.target.value)}
                            value={destination}
                            required>
                            <option value="" disabled>
                                -Select Destination-
                            </option>
                            {data.airports.map((airport) => (
                                <option key={airport.id} value={airport.id} disabled={airport.id == origin}>
                                    {airport.id} - {airport.name}, {airport.place}
                                </option>
                            ))}
                        </SelectionField>
                        <InputField
                            type="date"
                            name="Departure Date"
                            genDesign="w-full"
                            inDesign={`bg-blaze-tint`}
                            labDesign={`text-blaze-deep`}
                            min={data.date}
                            required
                        />
                        {flightMode == 1 ? (
                            <InputField
                                type="date"
                                name="Return Date"
                                genDesign="w-full"
                                inDesign={`bg-blaze-tint`}
                                labDesign={`text-blaze-deep`}
                                min={data.date}
                                required
                            />
                        ) : (
                            <></>
                        )}
                        <CounterField
                            name="Passengers"
                            genDesign="w-fit"
                            labDesign={`text-blaze-deep`}
                            min={1}
                            max={9}
                            value={passengers}
                            onChange={(num) => setPassengers(num)}
                        />
                    </div>
                </div>
                <div className="flex justify-center lg:h-10 lg:self-end lg:mt-auto mt-5">
                    <button type="submit" className="border flex justify-center items-center w-75 rounded-sm">
                        Search
                    </button>
                </div>
            </Form>
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
                    <div className="text-altitude-ink text-3xl font-bold">Places to Visit</div>
                    <h1 className="text-5xl font-bold text-cloud-pop">{places[selectedPlace].name}</h1>
                    <p className="mt-2 lg:w-1/2 lg:h-auto h-1/2 text-altitude-ink text-3xl">{places[selectedPlace].description}</p>
                </div>
            </div>
            <button
                className="absolute right-0 top-0 flex h-full items-center justify-center px-2 text-altitude-ink hover:text-blaze-core cursor-pointer"
                disabled={selectedPlace >= places.length - 1}
                onClick={() => {
                    setSelectedPlace(selectedPlace + 1);
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
