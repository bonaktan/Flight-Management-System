void saveStaffs() {
    ofstream f(FILE_STAFFS);
    for (auto& s : staffs)
        f << s.id << "|" << escape(s.name) << "|" << escape(s.current_location) << "|"
          << escape(s.role) << "|" << escape(s.schedule) << "|" << escape(s.created_at)
          << "\n";
}
void loadStaffs() {
    staffs.clear();
    ifstream f(FILE_STAFFS);
    string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        auto t = splitLine(line);
        if (t.size() < 6) continue;
        Staff s;
        s.id = stoll(t[0]);
        s.name = t[1];
        s.current_location = t[2];
        s.role = t[3];
        s.schedule = t[4];
        s.created_at = t[5];
        staffs.push_back(s);
    }
}

long long nextStaffId() {
    long long mx = 0;
    for (auto& s : staffs) mx = max(mx, s.id);
    return mx + 1;
}


void viewStaffs() {
    printHeader("STAFF");
    if (staffs.empty()) {
        cout << "  No records found.\n";
        return;
    }
    cout << "  " << left << setw(5) << "ID" << setw(22) << "Name" << setw(12)
         << "Location" << setw(20) << "Role" << setw(20) << "Schedule" << "\n";
    printDivider();
    for (auto& s : staffs)
        cout << "  " << setw(5) << s.id << setw(22) << s.name << setw(12)
             << s.current_location << setw(20) << s.role << setw(20)
             << s.schedule << "\n";
}
void addStaff() {
    printHeader("ADD STAFF");
    Staff s;
    s.id = nextStaffId();
    s.name = getInput("Name: ");
    s.current_location = getInput("Current Location (Airport ID): ");
    s.role = getInput("Role: ");
    s.schedule = getInput("Schedule (JSON string, e.g. {}): ");
    s.created_at = "NOW()";
    staffs.push_back(s);
    saveStaffs();
    cout << "\n  [OK] Staff added with ID " << s.id << "\n";
}
void modifyStaff() {
    printHeader("MODIFY STAFF");
    long long id = getLLInput("Enter Staff ID to modify: ");
    for (auto& s : staffs) {
        if (s.id == id) {
            string v;
            v = getInput("New Name [" + s.name + "]: ");
            if (!v.empty()) s.name = v;
            v = getInput("New Location [" + s.current_location + "]: ");
            if (!v.empty()) s.current_location = v;
            v = getInput("New Role [" + s.role + "]: ");
            if (!v.empty()) s.role = v;
            v = getInput("New Schedule [" + s.schedule + "]: ");
            if (!v.empty()) s.schedule = v;
            saveStaffs();
            cout << "\n  [OK] Staff updated.\n";
            return;
        }
    }
    cout << "\n  [!!] Staff not found.\n";
}
void deleteStaff() {
    printHeader("DELETE STAFF");
    long long id = getLLInput("Enter Staff ID to delete: ");
    auto it = remove_if(staffs.begin(), staffs.end(),
                        [id](const Staff& s) { return s.id == id; });
    if (it != staffs.end()) {
        staffs.erase(it, staffs.end());
        saveStaffs();
        cout << "\n  [OK] Staff deleted.\n";
    } else
        cout << "\n  [!!] Staff not found.\n";
}
