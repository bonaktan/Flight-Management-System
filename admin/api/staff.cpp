#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "../controls/controls.h"
#include "../display/display.h"
#include "./api.h"

using namespace Skybridge;

void API::Staff::save() {
    std::ofstream f(Data::FILE_STAFFS);
    for (auto& s : Data::staffs)
        f << s.id << "|" << Escape::escape(s.name) << "|"
          << Escape::escape(s.current_location) << "|" << Escape::escape(s.role)
          << "|" << Escape::escape(s.schedule) << "|"
          << Escape::escape(s.created_at) << "\n";
}

void API::Staff::load() {
    Data::staffs.clear();
    std::ifstream f(Data::FILE_STAFFS);
    std::string line;
    while (std::getline(f, line)) {
        if (line.empty()) continue;
        auto t = Escape::splitLine(line);
        if (t.size() < 6) continue;
        Structs::Staff s;
        try {
            s.id = Input::getLLInput(t[0]);
            s.name = t[1];
            s.current_location = t[2];
            s.role = t[3];
            s.schedule = t[4];
            s.created_at = t[5];
            Data::staffs.push_back(s);
        } catch (std::invalid_argument) {
            std::cerr << "Skipping Staff: Input - Not a Number" << std::endl;
        } catch (std::out_of_range) {
            std::cerr << "Skipping Staff: Input - Too Large: " << std::endl;
        }
    }
}

long long API::Staff::nextId() {
    long long mx = 0;
    for (auto& s : Data::staffs) mx = std::max(mx, s.id);
    return mx + 1;
}

void API::Staff::view() {
    Display::printHeader("STAFF");
    if (Data::staffs.empty()) {
        std::cout << "  No records found.\n";
        return;
    }
    std::cout << "  " << std::left << std::setw(5) << "ID" << std::setw(22)
              << "Name" << std::setw(12) << "Location" << std::setw(20)
              << "Role" << std::setw(20) << "Schedule" << "\n";
    Display::printDivider();
    for (auto& s : Data::staffs)
        std::cout << "  " << std::setw(5) << s.id << std::setw(22) << s.name
                  << std::setw(12) << s.current_location << std::setw(20)
                  << s.role << std::setw(20) << s.schedule << "\n";
}

void API::Staff::add() {
    Display::printHeader("ADD STAFF");
    Structs::Staff s;
    s.id = API::Staff::nextId();
    s.name = Input::getInput("Name: ");
    s.current_location = Input::getInput("Current Location (Airport ID): ");
    s.role = Input::getInput("Role: ");
    s.schedule = Input::getInput("Schedule (JSON string, e.g. {}): ");
    s.created_at = "NOW()";
    Data::staffs.push_back(s);
    API::Staff::save();
    std::cout << "\n  [OK] Staff added with ID " << s.id << "\n";
}

void API::Staff::modify() {
    Display::printHeader("MODIFY STAFF");
    long long id = Input::getLLInput("Enter Staff ID to modify: ");
    for (auto& s : Data::staffs) {
        if (s.id == id) {
            std::string v;
            v = Input::getInput("New Name [" + s.name + "]: ");
            if (!v.empty()) s.name = v;
            v = Input::getInput("New Location [" + s.current_location + "]: ");
            if (!v.empty()) s.current_location = v;
            v = Input::getInput("New Role [" + s.role + "]: ");
            if (!v.empty()) s.role = v;
            v = Input::getInput("New Schedule [" + s.schedule + "]: ");
            if (!v.empty()) s.schedule = v;
            API::Staff::save();
            std::cout << "\n  [OK] Staff updated.\n";
            return;
        }
    }
    std::cout << "\n  [!!] Staff not found.\n";
}

void API::Staff::remove() {
    Display::printHeader("DELETE STAFF");
    long long id = Input::getLLInput("Enter Staff ID to delete: ");
    auto it =
        std::remove_if(Data::staffs.begin(), Data::staffs.end(),
                       [id](const Structs::Staff& s) { return s.id == id; });
    if (it != Data::staffs.end()) {
        Data::staffs.erase(it, Data::staffs.end());
        API::Staff::save();
        std::cout << "\n  [OK] Staff deleted.\n";
    } else
        std::cout << "\n  [!!] Staff not found.\n";
}