import { use, useState } from "react";
import { SeatMapContext } from "./context";
import axios from "axios";
const apiUrl = import.meta.env.VITE_BACKEND_URL;

function StructuralZone({ zone }) {
    if (zone.label == "door")
        return (
            <div className="flex h-8 justify-between my-2">
                <div className="bg-red-300 rounded-r-lg w-4" />
                <div className="bg-red-300 rounded-l-lg w-4" />
            </div>
        );
    else if (zone.label == "lavatory")
        return (
            <div className="flex h-8 justify-between my-2">
                <div className="w-1/5 bg-orange-300" />
                <div>Toilet</div>
                <div className="w-1/5 bg-orange-300" />
            </div>
        );
    else if (zone.label == "galley") return <div className="h-8 bg-red-200 flex items-center justify-center my-2" />;
}

function SeatButton({ row, column }) {
    const seatmapLayout = use(SeatMapContext);
    const seatId = `${row}${column}`;
    const isOccupied = seatmapLayout.occupied_seats.includes(seatId);

    return (
        <button
            onClick={() => seatmapLayout.setSelectedSeat(seatId)}
            className={`text-sm aspect-square h-4/5 border ${isOccupied ? "bg-red-300" : seatmapLayout.selectedSeat == seatId ? "bg-orange-200" : "bg-green-300"}`}
            disabled={isOccupied}>
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
            <div className="text-xs px-2 items-center bg-blue-200">{zone.label}</div>
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

export async function loader() {
    let apiReturn;
    try {
        apiReturn = apiReturn = {
            aisleCount: 1,
            seatNumbering: { count: [3, 3], pattern: ["A", "B", "C", "AISLE", "D", "E", "F"] },
            zones: [
                { emergencyOnly: false, label: "door", type: "infra" },
                { label: "lavatory", type: "infra" },
                { label: "galley", type: "infra" },
                { colCount: 3, label: "Business Class", seatNumbering: { count: [2, 2], pattern: [["A", "C"], "AISLE", ["D", "F"]] }, type: "seat" },
                {
                    colCount: 8,
                    label: "Economy Class",
                    seatNumbering: { count: [3, 3], pattern: [["A", "B", "C"], "AISLE", ["D", "E", "F"]] },
                    type: "seat",
                },
                { emergencyOnly: false, label: "door", type: "infra" },
                {
                    colCount: 12,
                    label: "Economy Class",
                    seatNumbering: { count: [3, 3], pattern: [["A", "B", "C"], "AISLE", ["D", "E", "F"]] },
                    type: "seat",
                },
                { label: "galley", type: "infra" },
                { label: "lavatory", type: "infra" },
                { emergencyOnly: true, label: "door", type: "infra" },
            ],
            occupied_seats: ["A1"],
        };
    } catch (e) {
        console.error(e);
    }
    return { seatmapLayout: apiReturn };
}
export default function AircraftSeatmap({ loaderData }) {
    console.log(loaderData);
    const rowSections = getRowSections(loaderData.seatmapLayout.seatNumbering);
    const [selectedSeat, setSelectedSeat] = useState("");
    let previousSeat = 1;

    return (
        <SeatMapContext
            value={{ ...loaderData.seatmapLayout, rowSections: rowSections, selectedSeat: selectedSeat, setSelectedSeat: setSelectedSeat }}>
            <div id="seatmapBase">
                <div className="w-full max-w-sm bg-red-100">
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
        </SeatMapContext>
    );
}
