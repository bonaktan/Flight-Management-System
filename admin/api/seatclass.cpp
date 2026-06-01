void saveSeatClasses() {
    ofstream f(FILE_SEATCLASSES);
    for (auto& sc : seatClasses)
        f << sc.id << "|" << escape(sc.class_name) << "|" << sc.amt_of_seats << "|"
          << fixed << setprecision(2) << sc.markup_price << "|"
          << escape(sc.airplane_id) << "|" << escape(sc.created_at) << "\n";
}
void loadSeatClasses() {
    seatClasses.clear();
    ifstream f(FILE_SEATCLASSES);
    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        auto t = splitLine(line);
        if (t.size() < 6) continue;
        SeatClass sc;
        sc.id = stoll(t[0]);
        sc.class_name = t[1];
        sc.amt_of_seats = stoi(t[2]);
        sc.markup_price = stod(t[3]);
        sc.airplane_id = t[4];
        sc.created_at = t[5];
        seatClasses.push_back(sc);
    }
}

long long nextSeatClassId() {
    long long mx = 0;
    for (auto& s : seatClasses) mx = max(mx, s.id);
    return mx + 1;
}

void viewSeatClasses() {
    printHeader("SEAT CLASSES");
    if (seatClasses.empty()) {
        cout << "  No records found.\n";
        return;
    }
    cout << "  " << left << setw(5) << "ID" << setw(20) << "Class Name"
         << setw(10) << "# Seats" << setw(14) << "Markup" << setw(15)
         << "Airplane ID" << "\n";
    printDivider();
    for (auto& sc : seatClasses)
        cout << "  " << setw(5) << sc.id << setw(20) << sc.class_name
             << setw(10) << sc.amt_of_seats << setw(14) << fixed
             << setprecision(2) << sc.markup_price << setw(15) << sc.airplane_id
             << "\n";
}
void addSeatClass() {
    printHeader("ADD SEAT CLASS");
    SeatClass sc;
    sc.id = nextSeatClassId();
    sc.class_name = getInput("Class Name (e.g. Economy): ");
    sc.amt_of_seats = getIntInput("Number of Seats: ");
    sc.markup_price = getDoubleInput("Markup Price: ");
    sc.airplane_id = getInput("Airplane ID: ");
    sc.created_at = "NOW()";
    seatClasses.push_back(sc);
    saveSeatClasses();
    cout << "\n  [OK] Seat Class added with ID " << sc.id << "\n";
}
void modifySeatClass() {
    printHeader("MODIFY SEAT CLASS");
    long long id = getLLInput("Enter Seat Class ID to modify: ");
    for (auto& sc : seatClasses) {
        if (sc.id == id) {
            string v;
            v = getInput("New Class Name [" + sc.class_name + "]: ");
            if (!v.empty()) sc.class_name = v;
            string s =
                getInput("New # Seats [" + to_string(sc.amt_of_seats) + "]: ");
            if (!s.empty()) sc.amt_of_seats = stoi(s);
            string m = getInput("New Markup Price [" +
                                to_string(sc.markup_price) + "]: ");
            if (!m.empty()) sc.markup_price = stod(m);
            v = getInput("New Airplane ID [" + sc.airplane_id + "]: ");
            if (!v.empty()) sc.airplane_id = v;
            saveSeatClasses();
            cout << "\n  [OK] Seat Class updated.\n";
            return;
        }
    }
    cout << "\n  [!!] Seat Class not found.\n";
}
void deleteSeatClass() {
    printHeader("DELETE SEAT CLASS");
    long long id = getLLInput("Enter Seat Class ID to delete: ");
    auto it = remove_if(seatClasses.begin(), seatClasses.end(),
                        [id](const SeatClass& sc) { return sc.id == id; });
    if (it != seatClasses.end()) {
        seatClasses.erase(it, seatClasses.end());
        saveSeatClasses();
        cout << "\n  [OK] Seat Class deleted.\n";
    } else
        cout << "\n  [!!] Seat Class not found.\n";
}
