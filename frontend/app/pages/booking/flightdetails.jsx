// import { NavLink } from "react-router";

// export default function BookingFlightDetails() {
//     return (
//         <div>
//             <p>Booking - Flight Details (Step 2)</p>
//             <NavLink to="/booking/payment">Next</NavLink>
//         </div>
//     );
// }
/**
 * AircraftSeatmap — fully API-driven
 *
 * Expected API response shape (GET /api/flights/:flightId/seatmap):
 * {
 *   flight: {
 *     id: string,
 *     origin: string,       // e.g. "MNL"
 *     destination: string,  // e.g. "NRT"
 *     duration: string,     // e.g. "4h 30m"
 *     aircraft: string,     // e.g. "Airbus A320"
 *   },
 *   zones: Array<Zone>      // ordered front-to-back
 * }
 *
 * Zone (non-seat):
 * {
 *   type: "flightDeck" | "door" | "galley" | "lavatory",
 *   label: string
 * }
 *
 * Zone (seat section):
 * {
 *   type: "business" | "economy" | "exit",
 *   label: string,
 *   isExit?: boolean,
 *   layout: [number, number],   // seats left of aisle, seats right of aisle
 *   cols: string[],             // e.g. ["A","B","C","D","E","F"]
 *   rows: Array<{
 *     number: numbeearch?origin=MNL&destination=CEB&departure_date=2026-06-05&passengers=5&return_date=2026-06-30r,
 *     seats: Array<{
 *       id: string,             // e.g. "14C"
 *       col: string,
 *       status: "available" | "occupied"
 *     }>
 *   }>
 * }
 */

import { use, useState } from "react";
import { SeatMapContext } from "./context";

// ─── Mock API ────────────────────────────────────────────────────────────────
// Replace fetchSeatmap() with a real fetch() call to your endpoint.
// The shape below is the contract your backend should return.

const responseProto = {
    // airplane scope
    model: "Airbus A320",
    aisleCount: 1,
    seatNumbering: {
        pattern: ["A", "B", "C", "AISLE", "D", "E", "F"], // the pattern will be based from the compactest class (usually economy).
        count: [3, 3], // the number must match to how much item is in pattern
    },
    zones: [
        {
            type: "infra", // infra = plane infrastructure (entrance, lavatories)
            label: "door", // door infra: will have 2 unique entries: door labels. they have always entrances on both sides
            emergencyOnly: false, // false: can be used for normal ingress/egress. true: only used for emergency egress
        },
        {
            type: "infra",
            label: "lavatory", // lavatory infra: has 2 unique entries. will ALWAYS have 2 lavatories symmetrical
        },
        {
            type: "infra",
            label: "galley", // galley infra: has nothing special, it can probably be a big block i think
        },
        {
            type: "seat", // seat = usable seats for the user
            label: "First Class", // label can be modified for marketing, purely cosmetic
            seatNumbering: {
                pattern: [["A", "C"], "AISLE", ["E"]], // aisle marker is still required, even if its already implied in the numbering pattern. this also marks how much seats does a row have (compared against the base)
                count: [2, 1],
            },
            colCount: 2,
        },
        {
            type: "seat",
            label: "Business Class",
            seatNumbering: {
                pattern: [["A", "C"], "AISLE", ["D", "F"]], // aisle marker is still required, even if its already implied in the numbering pattern. this also marks how much seats does a row have (compared against the base)
                count: [2, 2],
            },
            colCount: 4, // how many times will the column repeat?
        },
        {
            type: "infra", // infra = plane infrastructure (entrance, lavatories)
            label: "door", // door infra: will have 2 unique entries: door labels. they have always entrances on both sides
            emergencyOnly: false, // false: can be used for normal ingress/egress. true: only used for emergency egress
        },
        {
            type: "seat",
            label: "Economy Class",
            seatNumbering: {
                pattern: [["A", "B", "C"], "AISLE", ["D", "E", "F"]], // the pattern will be based from the compactest class (usually economy).
                count: [3, 3], // the number must match to how much item is in pattern
            },
            colCount: 4,
        },
        {
            type: "infra", // infra = plane infrastructure (entrance, lavatories)
            label: "door", // door infra: will have 2 unique entries: door labels. they have always entrances on both sides
            emergencyOnly: false, // false: can be used for normal ingress/egress. true: only used for emergency egress
        },
        {
            type: "seat",
            label: "Economy Class",
            seatNumbering: {
                pattern: [["A", "B", "C"], "AISLE", ["D", "E", "F"]], // the pattern will be based from the compactest class (usually economy).
                count: [3, 3], // the number must match to how much item is in pattern
            },
            colCount: 4,
        },
        {
            type: "infra", // infra = plane infrastructure (entrance, lavatories)
            label: "door", // door infra: will have 2 unique entries: door labels. they have always entrances on both sides
            emergencyOnly: false, // false: can be used for normal ingress/egress. true: only used for emergency egress
        },
    ],
    occupiedSeats: ["A1", "A5", "A8"],
};
// const MOCK_RESPONSE = (() => {
//     const occupied = new Set([
//         "1A",
//         "1D",
//         "2C",
//         "3F",
//         "4A",
//         "4D",
//         "5B",
//         "6C",
//         "7A",
//         "7F",
//         "10A",
//         "10B",
//         "11C",
//         "11D",
//         "12A",
//         "12E",
//         "12F",
//         "14B",
//         "14D",
//         "16A",
//         "16C",
//         "17B",
//         "17F",
//         "18A",
//         "18D",
//         "19C",
//         "20B",
//         "20E",
//         "21A",
//         "21F",
//         "22C",
//         "22D",
//         "23B",
//         "24A",
//         "24F",
//         "25D",
//         "26B",
//         "26E",
//         "27A",
//         "28C",
//         "28F",
//         "29B",
//         "29D",
//         "30A",
//         "30F",
//     ]);

//     const makeRows = (rowStart, rowEnd, cols, skip = []) =>
//         Array.from({ length: rowEnd - rowStart + 1 }, (_, i) => rowStart + i)
//             .filter((r) => !skip.includes(r))
//             .map((number) => ({
//                 number,
//                 seats: cols.map((col) => ({
//                     id: `${number}${col}`,
//                     col,
//                     status: occupied.has(`${number}${col}`) ? "occupied" : "available",
//                 })),
//             }));

//     return {
//         zones: [
//             { type: "door", label: "Door 1L / 1R" },
//             { type: "galley", label: "Forward Galley" },
//             {
//                 type: "business",
//                 label: "Business Class",
//                 layout: [2, 2],
//                 cols: ["A", "C", "D", "F"],
//                 rows: makeRows(1, 7, ["A", "C", "D", "F"]),
//             },
//             { type: "galley", label: "Mid Galley + Lavatory" },
//             { type: "door", label: "Door 2L / 2R" },
//             {
//                 type: "economy",
//                 label: "Economy Class",
//                 layout: [3, 3],
//                 cols: ["A", "B", "C", "D", "E", "F"],
//                 rows: makeRows(10, 14, ["A", "B", "C", "D", "E", "F"]),
//             },
//             {
//                 type: "exit",
//                 label: "Emergency Exit",
//                 isExit: true,
//                 layout: [3, 3],
//                 cols: ["A", "B", "C", "D", "E", "F"],
//                 rows: makeRows(15, 16, ["A", "B", "C", "D", "E", "F"]),
//             },
//             {
//                 type: "economy",
//                 label: "Economy Class",
//                 layout: [3, 3],
//                 cols: ["A", "B", "C", "D", "E", "F"],
//                 rows: makeRows(17, 30, ["A", "B", "C", "D", "E", "F"]),
//             },
//             { type: "door", label: "Door 3L / 3R" },
//             { type: "lavatory", label: "Aft Lavatory" },
//             { type: "galley", label: "Aft Galley" },
//         ],
//     };
// })();

function StructuralZone({ zone }) {
    return (
        <div className={`flex h-8 bg-red-200 border`}>
            <div className="flex items-center gap-2">
                <p>{zone.label}</p>
            </div>
        </div>
    );
}

function SeatButton({ row, column }) {
    const seatmapLayout = use(SeatMapContext);
    const seatId = `${row}${column}`;
    const isOccupied = seatmapLayout.occupiedSeats.includes(seatId);

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
            <div
                className="text-xs flex justify-center items-center aspect-square"
                style={{ width: `${(1 / seatmapLayout.rowSections) * 100}%` }}></div>
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
            <div className="aspect-square" style={{ width: `${(1 / seatmapLayout.rowSections) * 100}%` }}></div>
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
        <div className="flex items-center justify-cente w-full">
            <div className="aspect-square w-full"></div>
            {aislePattern.pattern.map((row) => {
                if (row == "AISLE") row = "|";
                return (
                    <div key={row} className="flex items-center justify-center text-sm aspect-square w-full font-thin font-mono tracking-tighter">
                        {row}
                    </div>
                );
            })}
            <div className="aspect-square w-full"></div>
        </div>
    );
}

function getRowSections(seatNumbering) {
    const aisles = seatNumbering.count.length - 1;
    const seatRows = seatNumbering.count.reduce((accumulator, current) => accumulator + current, 0);
    return 1 + aisles + seatRows + 1;
}
// ─── Root ─────────────────────────────────────────────────────────────────────
export function loader() {
    const apiReturn = responseProto;
    return { seatmapLayout: apiReturn };
}
export default function AircraftSeatmap({ loaderData }) {
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
