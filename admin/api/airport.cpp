void saveAirports() {
    ofstream f(FILE_AIRPORTS);
    for (auto& a : airports)
        f << escape(a.id) << "|" << escape(a.name) << "|" << a.capacity << "|"
          << escape(a.created_at) << "\n";
}
void loadAirports() {
    airports.clear();
    ifstream f(FILE_AIRPORTS);
    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        auto t = splitLine(line);
        if (t.size() < 4) continue;
        Airport a;
        a.id = t[0];
        a.name = t[1];
        a.capacity = stoi(t[2]);
        a.created_at = t[3];
        airports.push_back(a);
    }
}

void viewAirports() {
    printHeader("AIRPORTS");
    if (airports.empty()) {
        cout << "  No records found.\n";
        return;
    }
    cout << "  " << left << setw(12) << "ID" << setw(32) << "Name" << setw(10)
         << "Capacity" << "\n";
    printDivider();
    for (auto& a : airports)
        cout << "  " << setw(12) << a.id << setw(32) << a.name << setw(10)
             << a.capacity << "\n";
}
void addAirport() {
    printHeader("ADD AIRPORT");
    Airport a;
    a.id = getInput("Airport ID (e.g. MNL): ");
    a.name = getInput("Airport Name: ");
    a.capacity = getIntInput("Capacity: ");
    a.created_at = "NOW()";
    airports.push_back(a);
    saveAirports();
    cout << "\n  [OK] Airport added.\n";
}
void modifyAirport() {
    printHeader("MODIFY AIRPORT");
    string id = getInput("Enter Airport ID to modify: ");
    for (auto& a : airports) {
        if (a.id == id) {
            string v;
            v = getInput("New Name [" + a.name + "]: ");
            if (!v.empty()) a.name = v;
            string c =
                getInput("New Capacity [" + to_string(a.capacity) + "]: ");
            if (!c.empty()) a.capacity = stoi(c);
            saveAirports();
            cout << "\n  [OK] Airport updated.\n";
            return;
        }
    }
    cout << "\n  [!!] Airport not found.\n";
}
void deleteAirport() {
    printHeader("DELETE AIRPORT");
    string id = getInput("Enter Airport ID to delete: ");
    auto it = remove_if(airports.begin(), airports.end(),
                        [&id](const Airport& a) { return a.id == id; });
    if (it != airports.end()) {
        airports.erase(it, airports.end());
        saveAirports();
        cout << "\n  [OK] Airport deleted.\n";
    } else
        cout << "\n  [!!] Airport not found.\n";
}