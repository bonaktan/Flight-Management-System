void saveFlightStaffs() {
    ofstream f(FILE_FLIGHTSTAFF);
    for (auto& fs : flightStaffs)
        f << escape(fs.flight_id) << "|" << fs.staff_id << "\n";
}
void loadFlightStaffs() {
    flightStaffs.clear();
    ifstream f(FILE_FLIGHTSTAFF);
    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        auto t = splitLine(line);
        if (t.size() < 2) continue;
        FlightStaff fs;
        fs.flight_id = t[0];
        fs.staff_id = stoll(t[1]);
        flightStaffs.push_back(fs);
    }
}

void viewFlightStaffs() {
    printHeader("FLIGHT STAFF (Junction)");
    if (flightStaffs.empty()) {
        cout << "  No records found.\n";
        return;
    }
    cout << "  " << left << setw(16) << "Flight ID" << setw(12) << "Staff ID"
         << "\n";
    printDivider();
    for (auto& fs : flightStaffs)
        cout << "  " << setw(16) << fs.flight_id << setw(12) << fs.staff_id
             << "\n";
}
void addFlightStaff() {
    printHeader("ADD FLIGHT STAFF");
    FlightStaff fs;
    fs.flight_id = getInput("Flight ID: ");
    fs.staff_id = getLLInput("Staff ID: ");
    flightStaffs.push_back(fs);
    saveFlightStaffs();
    cout << "\n  [OK] Flight-Staff link added.\n";
}
void modifyFlightStaff() {
    printHeader("MODIFY FLIGHT STAFF");
    string fid = getInput("Flight ID of record to find: ");
    long long sid = getLLInput("Staff ID of record to find: ");
    for (auto& fs : flightStaffs) {
        if (fs.flight_id == fid && fs.staff_id == sid) {
            string v = getInput("New Flight ID [" + fs.flight_id + "]: ");
            if (!v.empty()) fs.flight_id = v;
            string ns =
                getInput("New Staff ID [" + to_string(fs.staff_id) + "]: ");
            if (!ns.empty()) fs.staff_id = stoll(ns);
            saveFlightStaffs();
            cout << "\n  [OK] Record updated.\n";
            return;
        }
    }
    cout << "\n  [!!] Record not found.\n";
}
void deleteFlightStaff() {
    printHeader("DELETE FLIGHT STAFF");
    string fid = getInput("Flight ID: ");
    long long sid = getLLInput("Staff ID: ");
    auto it = remove_if(flightStaffs.begin(), flightStaffs.end(),
                        [&fid, sid](const FlightStaff& fs) {
                            return fs.flight_id == fid && fs.staff_id == sid;
                        });
    if (it != flightStaffs.end()) {
        flightStaffs.erase(it, flightStaffs.end());
        saveFlightStaffs();
        cout << "\n  [OK] Record deleted.\n";
    } else
        cout << "\n  [!!] Record not found.\n";
}