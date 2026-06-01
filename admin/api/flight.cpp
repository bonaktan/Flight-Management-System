void saveFlights() {
    ofstream f(FILE_FLIGHTS);
    for (auto& fl : flights)
        f << escape(fl.id) << "|" << escape(fl.departure_airport_id) << "|"
          << escape(fl.arrival_airport_id) << "|" << fixed << setprecision(2)
          << fl.base_ticket_price << "|" << escape(fl.flight_time) << "|"
          << escape(fl.departure) << "|" << escape(fl.frequency) << "|"
          << escape(fl.created_at) << "\n";
}
void loadFlights() {
    flights.clear();
    ifstream f(FILE_FLIGHTS);
    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        auto t = splitLine(line);
        if (t.size() < 8) continue;
        Flight fl;
        fl.id = t[0];
        fl.departure_airport_id = t[1];
        fl.arrival_airport_id = t[2];
        fl.base_ticket_price = stod(t[3]);
        fl.flight_time = t[4];
        fl.departure = t[5];
        fl.frequency = t[6];
        fl.created_at = t[7];
        flights.push_back(fl);
    }
}

// FLIGHT CRUD
void viewFlights() {
    printHeader("FLIGHTS");
    if (flights.empty()) {
        cout << "  No records found.\n";
        return;
    }
    cout << "  " << left << setw(12) << "ID" << setw(10) << "Dep.AP" << setw(10)
         << "Arr.AP" << setw(12) << "Price" << setw(12) << "Duration"
         << setw(22) << "Departure" << setw(12) << "Frequency" << "\n";
    printDivider();
    for (auto& fl : flights)
        cout << "  " << setw(12) << fl.id << setw(10) << fl.departure_airport_id
             << setw(10) << fl.arrival_airport_id << setw(12) << fixed
             << setprecision(2) << fl.base_ticket_price << setw(12)
             << fl.flight_time << setw(22) << fl.departure << setw(12)
             << fl.frequency << "\n";
}
void addFlight() {
    printHeader("ADD FLIGHT");
    Flight fl;
    fl.id = getInput("Flight ID (e.g. PR101): ");
    fl.departure_airport_id = getInput("Departure Airport ID: ");
    fl.arrival_airport_id = getInput("Arrival Airport ID: ");
    fl.base_ticket_price = getDoubleInput("Base Ticket Price: ");
    fl.flight_time = getInput("Flight Time (e.g. 02:30:00): ");
    fl.departure = getInput("Departure (YYYY-MM-DD HH:MM:SS): ");
    fl.frequency = getInput("Frequency (e.g. 7 days): ");
    fl.created_at = "NOW()";
    flights.push_back(fl);
    saveFlights();
    cout << "\n  [OK] Flight added.\n";
}
void modifyFlight() {
    printHeader("MODIFY FLIGHT");
    string id = getInput("Enter Flight ID to modify: ");
    for (auto& fl : flights) {
        if (fl.id == id) {
            string v;
            v = getInput("New Dep Airport [" + fl.departure_airport_id + "]: ");
            if (!v.empty()) fl.departure_airport_id = v;
            v = getInput("New Arr Airport [" + fl.arrival_airport_id + "]: ");
            if (!v.empty()) fl.arrival_airport_id = v;
            string pr = getInput("New Price [" +
                                 to_string(fl.base_ticket_price) + "]: ");
            if (!pr.empty()) fl.base_ticket_price = stod(pr);
            v = getInput("New Flight Time [" + fl.flight_time + "]: ");
            if (!v.empty()) fl.flight_time = v;
            v = getInput("New Departure [" + fl.departure + "]: ");
            if (!v.empty()) fl.departure = v;
            v = getInput("New Frequency [" + fl.frequency + "]: ");
            if (!v.empty()) fl.frequency = v;
            saveFlights();
            cout << "\n  [OK] Flight updated.\n";
            return;
        }
    }
    cout << "\n  [!!] Flight not found.\n";
}
void deleteFlight() {
    printHeader("DELETE FLIGHT");
    string id = getInput("Enter Flight ID to delete: ");
    auto it = remove_if(flights.begin(), flights.end(),
                        [&id](const Flight& fl) { return fl.id == id; });
    if (it != flights.end()) {
        flights.erase(it, flights.end());
        saveFlights();
        cout << "\n  [OK] Flight deleted.\n";
    } else
        cout << "\n  [!!] Flight not found.\n";
}
