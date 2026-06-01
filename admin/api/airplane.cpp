void saveAirplanes() {
    ofstream f(FILE_AIRPLANES);
    for (auto& a : airplanes)
        f << escape(a.id) << "|" << escape(a.model) << "|" << escape(a.location) << "\n";
}
void loadAirplanes() {
    airplanes.clear();
    ifstream f(FILE_AIRPLANES);
    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        auto t = splitLine(line);
        if (t.size() < 3) continue;
        Airplane a;
        a.id = t[0];
        a.model = t[1];
        a.location = t[2];
        airplanes.push_back(a);
    }
}

void viewAirplanes() {
    printHeader("AIRPLANES");
    if (airplanes.empty()) {
        cout << "  No records found.\n";
        return;
    }
    cout << "  " << left << setw(15) << "ID" << setw(28) << "Model" << setw(12)
         << "Location" << "\n";
    printDivider();
    for (auto& a : airplanes)
        cout << "  " << setw(15) << a.id << setw(28) << a.model << setw(12)
             << a.location << "\n";
}
void addAirplane() {
    printHeader("ADD AIRPLANE");
    Airplane a;
    a.id = getInput("Airplane ID (e.g. RP-C8888): ");
    a.model = getInput("Model (e.g. Boeing 737): ");
    a.location = getInput("Location (Airport ID, or blank): ");
    airplanes.push_back(a);
    saveAirplanes();
    cout << "\n  [OK] Airplane added.\n";
}
void modifyAirplane() {
    printHeader("MODIFY AIRPLANE");
    string id = getInput("Enter Airplane ID to modify: ");
    for (auto& a : airplanes) {
        if (a.id == id) {
            string v;
            v = getInput("New Model [" + a.model + "]: ");
            if (!v.empty()) a.model = v;
            v = getInput("New Location [" + a.location + "]: ");
            if (!v.empty()) a.location = v;
            saveAirplanes();
            cout << "\n  [OK] Airplane updated.\n";
            return;
        }
    }
    cout << "\n  [!!] Airplane not found.\n";
}
void deleteAirplane() {
    printHeader("DELETE AIRPLANE");
    string id = getInput("Enter Airplane ID to delete: ");
    auto it = remove_if(airplanes.begin(), airplanes.end(),
                        [&id](const Airplane& a) { return a.id == id; });
    if (it != airplanes.end()) {
        airplanes.erase(it, airplanes.end());
        saveAirplanes();
        cout << "\n  [OK] Airplane deleted.\n";
    } else
        cout << "\n  [!!] Airplane not found.\n";
}