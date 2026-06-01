void saveAirportFlights() {
    ofstream f(FILE_AIRPORTFLIGHT);
    for (auto& af : airportFlights)
        f << escape(af.airport_id) << "|" << escape(af.flight_id) << "|"
          << escape(af.relationship_type) << "\n";
}
void loadAirportFlights() {
    airportFlights.clear();
    ifstream f(FILE_AIRPORTFLIGHT);
    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        auto t = splitLine(line);
        if (t.size() < 3) continue;
        AirportFlight af;
        af.airport_id = t[0];
        af.flight_id = t[1];
        af.relationship_type = t[2];
        airportFlights.push_back(af);
    }
}

void viewAirportFlights() {
    printHeader("AIRPORT FLIGHT (Junction)");
    if (airportFlights.empty()) {
        cout << "  No records found.\n";
        return;
    }
    cout << "  " << left << setw(14) << "Airport ID" << setw(14) << "Flight ID"
         << setw(20) << "Relationship" << "\n";
    printDivider();
    for (auto& af : airportFlights)
        cout << "  " << setw(14) << af.airport_id << setw(14) << af.flight_id
             << setw(20) << af.relationship_type << "\n";
}
void addAirportFlight() {
    printHeader("ADD AIRPORT FLIGHT");
    AirportFlight af;
    af.airport_id = getInput("Airport ID: ");
    af.flight_id = getInput("Flight ID: ");
    af.relationship_type =
        getInput("Relationship Type (e.g. departure/arrival): ");
    airportFlights.push_back(af);
    saveAirportFlights();
    cout << "\n  [OK] Airport-Flight link added.\n";
}
void modifyAirportFlight() {
    printHeader("MODIFY AIRPORT FLIGHT");
    string aid = getInput("Airport ID of record to find: ");
    string fid = getInput("Flight ID of record to find: ");
    string rel = getInput("Relationship Type of record to find: ");
    for (auto& af : airportFlights) {
        if (af.airport_id == aid && af.flight_id == fid &&
            af.relationship_type == rel) {
            string v;
            v = getInput("New Airport ID [" + af.airport_id + "]: ");
            if (!v.empty()) af.airport_id = v;
            v = getInput("New Flight ID [" + af.flight_id + "]: ");
            if (!v.empty()) af.flight_id = v;
            v = getInput("New Relationship Type [" + af.relationship_type +
                         "]: ");
            if (!v.empty()) af.relationship_type = v;
            saveAirportFlights();
            cout << "\n  [OK] Record updated.\n";
            return;
        }
    }
    cout << "\n  [!!] Record not found.\n";
}
void deleteAirportFlight() {
    printHeader("DELETE AIRPORT FLIGHT");
    string aid = getInput("Airport ID: ");
    string fid = getInput("Flight ID: ");
    string rel = getInput("Relationship Type: ");
    auto it = remove_if(airportFlights.begin(), airportFlights.end(),
                        [&](const AirportFlight& af) {
                            return af.airport_id == aid &&
                                   af.flight_id == fid &&
                                   af.relationship_type == rel;
                        });
    if (it != airportFlights.end()) {
        airportFlights.erase(it, airportFlights.end());
        saveAirportFlights();
        cout << "\n  [OK] Record deleted.\n";
    } else
        cout << "\n  [!!] Record not found.\n";
}