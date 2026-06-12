import { use, useState } from "react";
import { BookingContext, SeatMapContext } from "./context";
import axios from "axios";
import { Link, useNavigate } from "react-router";

export function meta() {
    return [{ title: "Booking - Flight Details | SkyBridge Airlines" }];
}

function StructuralZone({ zone }) {
    if (zone.label == "door")
        return (
            <div className="flex h-8 justify-between my-2">
                <div className="bg-cloud-pop rounded-r-lg w-4" />
                <div className="bg-cloud-pop rounded-l-lg w-4" />
            </div>
        );
    else if (zone.label == "lavatory")
        return (
            <div className="flex h-8 justify-between my-2">
                <div className="w-1/5 bg-cloud-warm" />
                <div>Toilet</div>
                <div className="w-1/5 bg-cloud-warm" />
            </div>
        );
    else if (zone.label == "galley") return <div className="h-8 bg-altitude-haze flex items-center justify-center my-2" />;
}

function SeatButton({ row, column }) {
    const seatmapLayout = use(SeatMapContext);
    const bookingContext = use(BookingContext);
    const seatId = `${row}${column}`;
    const occupiedSeats = bookingContext.passengers.map((p) => p.selected_seat);
    const isOccupied = seatmapLayout.occupied_seats.includes(seatId);
    const isOccupiedByOthers = occupiedSeats.includes(seatId);
    function onClick(e) {
        e.preventDefault();
        bookingContext.setBookingContext({
            field: "passengers",
            count: seatmapLayout.selectedPassenger,
            subField: "selected_seat",
            value: seatId,
        });
    }
    return (
        <button
            onClick={onClick}
            type="button"
            className={`text-sm rounded-sm aspect-square h-4/5 border text-altitude-ink ${isOccupied ? "bg-red-300" : bookingContext.passengers[seatmapLayout.selectedPassenger].selected_seat == seatId ? "bg-orange-300" : isOccupiedByOthers ? "bg-orange-200" : "bg-green-300"}`}
            disabled={isOccupied || isOccupiedByOthers}>
            {row}
        </button>
    );
}

function SeatRow({ columnCount, pattern }) {
    const seatmapLayout = use(SeatMapContext);
    return (
        <div className="flex items-stretch">
            {pattern.pattern.map((seat, i) => {
                if (seat == "AISLE")
                    return (
                        <div
                            key={i}
                            className="text-xs flex justify-center items-center aspect-square"
                            style={{ width: `${(1 / seatmapLayout.rowSections) * 100}%` }}>
                            {columnCount}
                        </div>
                    );
                else
                    return (
                        <div
                            key={i}
                            className="flex self-stretch items-center justify-evenly"
                            style={{ width: `${(seatmapLayout.seatNumbering.count[0] / seatmapLayout.rowSections) * 100}%` }}>
                            {seat.map((row, i) => (
                                <SeatButton key={i} row={row} column={columnCount} />
                            ))}
                        </div>
                    );
            })}
        </div>
    );
}

function SeatZone({ zone, startingSeatCount }) {
    return (
        <div className="">
            <div className="text-xs py-1 px-2 items-center bg-blue-200 text-altitude-ink">{zone.label}</div>
            {Array.from({ length: zone.colCount }, (_, i) => startingSeatCount + i).map((columnCount) => (
                <SeatRow key={columnCount} columnCount={columnCount} pattern={zone.seatNumbering} />
            ))}
        </div>
    );
}

function ColHeaders({ aislePattern }) {
    return (
        <div className="flex items-center justify-center w-full">
            {aislePattern.pattern.map((row) => {
                if (row == "AISLE") row = "|";
                return (
                    <div key={row} className="flex items-center justify-center text-sm aspect-square w-full font-thin font-mono tracking-tighter">
                        {row}
                    </div>
                );
            })}
        </div>
    );
}

function getRowSections(seatNumbering) {
    const aisles = seatNumbering.count.length - 1;
    const seatRows = seatNumbering.count.reduce((accumulator, current) => accumulator + current, 0);
    return aisles + seatRows;
}

function PassengerLoader({ id, passenger, onSelect, onClick }) {
    const Meals = ["Sandwich", "Drink", "Snack"];
    const seatMapContext = use(SeatMapContext);
    const isExpanded = id == seatMapContext.selectedPassenger;
    const bookingContext = use(BookingContext);
    return (
        <div
            className={`bg-blaze-core text-cloud-warm rounded-sm p-2 overflow-hidden transition duration-1000 ${isExpanded ? "max-h-40" : "max-h-17"}`}
            onClick={() => onSelect(id)}>
            <div className="flex items-center gap-6 pl-4 py-4">
                <div className={`h-20 aspect-square bg-white rounded-full ${!isExpanded && "opacity-0"}`}>
                    <svg viewBox="0 0 338 338" width="100%" height="100%" xmlns="http://www.w3.org/2000/svg">
                        <path fill="#DDD" d="m169,.5a169,169 0 1,0 2,0zm0,86a76,76 0 1 1-2,0zM57,287q27-35 67-35h92q40,0 67,35a164,164 0 0,1-226,0" />
                    </svg>
                </div>
                <div>
                    <div className="font-medium text-xl ">
                        {passenger.title}. {passenger.first_name} {passenger.last_name}
                    </div>
                    <div className={`overflow-hidden transition ${isExpanded ? "opacity-100" : "opacity-0"}`}>
                        <div>Seat: {bookingContext.passengers[id].selected_seat}</div>
                    </div>
                </div>
            </div>
        </div>
    );
}

export async function clientLoader() {
    let apiReturn;
    const bookingContext = JSON.parse(sessionStorage.getItem("bookingState"));
    try {
        apiReturn = await axios.get("/api/search/airplane/seatmap", {
            params: { flight: bookingContext.flightId, departure_date: bookingContext.departure_date },
        });
    } catch (e) {
        console.error(e);
    }
    return { seatmapLayout: apiReturn.data };
}

export default function AircraftSeatmap({ loaderData }) {
    const bookingContext = use(BookingContext);
    const navigate = useNavigate();
    const rowSections = getRowSections(loaderData.seatmapLayout.seatNumbering);
    const [selectedPassenger, setSelectedPassenger] = useState(0);
    const [notSelected, setNotSelected] = useState();
    function onPassengerSelect(count) {
        setSelectedPassenger(count);
    }
    let previousSeat = 1;
    const [expandedPassenger, setExpandedPassenger] = useState(null);

    const togglePassenger = (id) => {
        setExpandedPassenger((prev) => (prev === id ? null : id));
    };

    function onNext() {
        if (!bookingContext.passengers.every((p) => p.selected_seat !== "")) {
            setNotSelected(
                bookingContext.passengers.filter((p) => !p.selected_seat).map((p, i) => `Passenger ${i + 1}: ${p.first_name} ${p.last_name}`),
            );
            return;
        }
        navigate("/booking/payment");
    }
    return (
        <SeatMapContext
            value={{
                ...loaderData.seatmapLayout,
                rowSections: rowSections,
                selectedPassenger: selectedPassenger,
                setSelectedPassenger: setSelectedPassenger,
            }}>
            <div id="seatmapBase" className="w-full flex justify-center gap-10">
                <div className="flex flex-col w-1/2 p-2">
                    <p className="font-bold text-2xl pb-4">Passengers:</p>
                    <div className="px-6 py-10 bg-blaze-tint rounded-md ">
                        <div className="flex flex-col gap-2">
                            {bookingContext.passengers.map((passenger, key) => (
                                <PassengerLoader key={key} id={key} passenger={passenger} onSelect={onPassengerSelect} />
                            ))}
                        </div>
                        <div className="flex justify-end-safe p-2 pt-10 text-cloud-warm">
                            <button onClick={onNext} className="bg-blaze-core px-4 py-2 text-xl">
                                Next
                            </button>
                        </div>
                        {notSelected && (
                            <div>
                                <p>These passengers still have no seats assigned to them:</p>
                                {notSelected.map((e, key) => (
                                    <p key={e}>{e}</p>
                                ))}
                            </div>
                        )}
                    </div>
                </div>
                <div className="flex max-h-[80dvh] w-1/3 border bg-altitude-ink p-2 h-full overflow-hidden">
                    <div id="legend" className="w-1/3 bg-altitude-ink text-cloud-warm">
                        <p className=" p-2 font-bold ">Legend:</p>
                        <div className="flex items-center gap-2 p-2">
                            <div className="w-4 h-4 bg-green-300" />
                            <div>Available</div>
                        </div>
                        <div className="flex items-center gap-2 p-2">
                            <div className="w-4 h-4 bg-orange-200" />
                            <div>Selected</div>
                        </div>
                        <div className="flex items-center gap-2 p-2">
                            <div className="w-4 h-4 bg-red-300" />
                            <div>Occupied</div>
                        </div>
                        <div className="flex items-center gap-2 p-2">
                            <div className="w-4 h-4 bg-cloud-warm" />
                            <div>Toilet</div>
                        </div>
                        <div className="flex items-center gap-2 p-2">
                            <div className="w-4 h-4 bg-cloud-pop" />
                            <div>Exits</div>
                        </div>
                        <div className="flex items-center gap-2 p-2">
                            <div className="w-4 h-4 bg-altitude-haze" />
                            <div>Galleys</div>
                        </div>
                    </div>
                    <div className="w-2/3 max-w-sm bg-altitude-ink text-cloud-warm overflow-auto p-2">
                        <ColHeaders aislePattern={loaderData.seatmapLayout.seatNumbering} />
                        <hr />
                        {loaderData.seatmapLayout.zones.map((zone, _key) => {
                            const key = `${zone.type}.${zone.label}.${_key}`;
                            if (zone.type == "infra") return <StructuralZone key={key} zone={zone} />;
                            else if (zone.type == "seat") {
                                let currentSeat = previousSeat;
                                previousSeat += zone.colCount;
                                return <SeatZone key={key} zone={zone} startingSeatCount={currentSeat} />;
                            }
                        })}
                    </div>
                </div>
            </div>
        </SeatMapContext>
    );
}
