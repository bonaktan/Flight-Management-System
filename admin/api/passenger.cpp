void savePassengers() {
    ofstream f(FILE_PASSENGERS);
    for (auto& p : passengers)
        f << p.id << "|" << escape(p.frequent_flyer_code) << "|" << escape(p.title)
          << "|" << escape(p.first_name) << "|" << escape(p.last_name) << "|"
          << escape(p.birthdate) << "|" << escape(p.contact_email) << "|"
          << escape(p.emergency_contact_name) << "|"
          << escape(p.emergency_contact_email) << "|" << p.associated_to << "|"
          << escape(p.created_at) << "|" << escape(p.updated_at) << "\n";
}
void loadPassengers() {
    passengers.clear();
    ifstream f(FILE_PASSENGERS);
    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        auto t = splitLine(line);
        if (t.size() < 12) continue;
        Passenger p;
        p.id = stoll(t[0]);
        p.frequent_flyer_code = t[1];
        p.title = t[2];
        p.first_name = t[3];
        p.last_name = t[4];
        p.birthdate = t[5];
        p.contact_email = t[6];
        p.emergency_contact_name = t[7];
        p.emergency_contact_email = t[8];
        p.associated_to = stoll(t[9]);
        p.created_at = t[10];
        p.updated_at = t[11];
        passengers.push_back(p);
    }
}

long long nextPassengerId() {
    long long mx = 0;
    for (auto& p : passengers) mx = max(mx, p.id);
    return mx + 1;
}

void viewPassengers() {
    printHeader("PASSENGERS");
    if (passengers.empty()) {
        cout << "  No records found.\n";
        return;
    }
    cout << "  " << left << setw(5) << "ID" << setw(8) << "Title" << setw(15)
         << "First" << setw(15) << "Last" << setw(12) << "Birthdate" << setw(26)
         << "Email" << setw(10) << "AcctID" << "\n";
    printDivider();
    for (auto& p : passengers)
        cout << "  " << setw(5) << p.id << setw(8) << p.title << setw(15)
             << p.first_name << setw(15) << p.last_name << setw(12)
             << p.birthdate << setw(26) << p.contact_email << setw(10)
             << p.associated_to << "\n";
}
void addPassenger() {
    printHeader("ADD PASSENGER");
    Passenger p;
    p.id = nextPassengerId();
    p.frequent_flyer_code = getInput("Frequent Flyer Code (blank if none): ");
    p.title = getInput("Title (Mr/Ms/Dr...): ");
    p.first_name = getInput("First Name: ");
    p.last_name = getInput("Last Name: ");
    p.birthdate = getInput("Birthdate (YYYY-MM-DD): ");
    p.contact_email = getInput("Contact Email: ");
    p.emergency_contact_name = getInput("Emergency Contact Name: ");
    p.emergency_contact_email = getInput("Emergency Contact Email: ");
    p.associated_to = getLLInput("Associated Account ID: ");
    p.created_at = p.updated_at = "NOW()";
    passengers.push_back(p);
    savePassengers();
    cout << "\n  [OK] Passenger added with ID " << p.id << "\n";
}
void modifyPassenger() {
    printHeader("MODIFY PASSENGER");
    long long id = getLLInput("Enter Passenger ID to modify: ");
    for (auto& p : passengers) {
        if (p.id == id) {
            string v;
            v = getInput("New FFC [" + p.frequent_flyer_code + "]: ");
            if (!v.empty()) p.frequent_flyer_code = v;
            v = getInput("New Title [" + p.title + "]: ");
            if (!v.empty()) p.title = v;
            v = getInput("New First Name [" + p.first_name + "]: ");
            if (!v.empty()) p.first_name = v;
            v = getInput("New Last Name [" + p.last_name + "]: ");
            if (!v.empty()) p.last_name = v;
            v = getInput("New Birthdate [" + p.birthdate + "]: ");
            if (!v.empty()) p.birthdate = v;
            v = getInput("New Email [" + p.contact_email + "]: ");
            if (!v.empty()) p.contact_email = v;
            v = getInput("New Emrg Name [" + p.emergency_contact_name + "]: ");
            if (!v.empty()) p.emergency_contact_name = v;
            v = getInput("New Emrg Email [" + p.emergency_contact_email +
                         "]: ");
            if (!v.empty()) p.emergency_contact_email = v;
            p.updated_at = "NOW()";
            savePassengers();
            cout << "\n  [OK] Passenger updated.\n";
            return;
        }
    }
    cout << "\n  [!!] Passenger not found.\n";
}
void deletePassenger() {
    printHeader("DELETE PASSENGER");
    long long id = getLLInput("Enter Passenger ID to delete: ");
    auto it = remove_if(passengers.begin(), passengers.end(),
                        [id](const Passenger& p) { return p.id == id; });
    if (it != passengers.end()) {
        passengers.erase(it, passengers.end());
        savePassengers();
        cout << "\n  [OK] Passenger deleted.\n";
    } else
        cout << "\n  [!!] Passenger not found.\n";
}