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

void API::Passenger::save() {
    std::ofstream f(Data::FILE_PASSENGERS);
    for (auto& p : Data::passengers)
        f << p.id << "|" << Escape::escape(p.frequent_flyer_code) << "|"
          << Escape::escape(p.title) << "|" << Escape::escape(p.first_name)
          << "|" << Escape::escape(p.last_name) << "|"
          << Escape::escape(p.birthdate) << "|"
          << Escape::escape(p.contact_email) << "|"
          << Escape::escape(p.emergency_contact_name) << "|"
          << Escape::escape(p.emergency_contact_email) << "|" << p.associated_to
          << "|" << Escape::escape(p.created_at) << "|"
          << Escape::escape(p.updated_at) << "\n";
}

void API::Passenger::load() {
    Data::passengers.clear();
    std::ifstream f(Data::FILE_PASSENGERS);
    std::string line;
    while (std::getline(f, line)) {
        if (line.empty()) continue;
        auto t = Escape::splitLine(line);
        if (t.size() < 12) continue;
        Structs::Passenger p;
        try {
            p.id = Input::getLLInput(t[0]);
            p.frequent_flyer_code = t[1];
            p.title = t[2];
            p.first_name = t[3];
            p.last_name = t[4];
            p.birthdate = t[5];
            p.contact_email = t[6];
            p.emergency_contact_name = t[7];
            p.emergency_contact_email = t[8];
            p.associated_to = Input::getLLInput(t[9]);
            p.created_at = t[10];
            p.updated_at = t[11];
            Data::passengers.push_back(p);
        } catch (std::invalid_argument) {
            std::cerr << "Skipping Passenger: Input - Not a Number" << std::endl;
        } catch (std::out_of_range) {
            std::cerr << "Skipping User: Input - Too Large: " << std::endl;
        }
    }
}

long long API::Passenger::nextId() {
    long long mx = 0;
    for (auto& p : Data::passengers) mx = std::max(mx, p.id);
    return mx + 1;
}

void API::Passenger::view() {
    Display::printHeader("PASSENGERS");
    if (Data::passengers.empty()) {
        std::cout << "  No records found.\n";
        return;
    }
    std::cout << "  " << std::left << std::setw(5) << "ID" << std::setw(8)
              << "Title" << std::setw(15) << "First" << std::setw(15) << "Last"
              << std::setw(12) << "Birthdate" << std::setw(26) << "Email"
              << std::setw(10) << "AcctID" << "\n";
    Display::printDivider();
    for (auto& p : Data::passengers)
        std::cout << "  " << std::setw(5) << p.id << std::setw(8) << p.title
                  << std::setw(15) << p.first_name << std::setw(15)
                  << p.last_name << std::setw(12) << p.birthdate
                  << std::setw(26) << p.contact_email << std::setw(10)
                  << p.associated_to << "\n";
}

void API::Passenger::add() {
    Display::printHeader("ADD PASSENGER");
    Structs::Passenger p;
    p.id = API::Passenger::nextId();
    p.frequent_flyer_code =
        Input::getInput("Frequent Flyer Code (blank if none): ");
    p.title = Input::getInput("Title (Mr/Ms/Dr...): ");
    p.first_name = Input::getInput("First Name: ");
    p.last_name = Input::getInput("Last Name: ");
    p.birthdate = Input::getInput("Birthdate (YYYY-MM-DD): ");
    p.contact_email = Input::getInput("Contact Email: ");
    p.emergency_contact_name = Input::getInput("Emergency Contact Name: ");
    p.emergency_contact_email = Input::getInput("Emergency Contact Email: ");
    p.associated_to = Input::getLLInput("Associated Account ID: ");
    p.created_at = p.updated_at = "NOW()";
    Data::passengers.push_back(p);
    API::Passenger::save();
    std::cout << "\n  [OK] Passenger added with ID " << p.id << "\n";
}

void API::Passenger::modify() {
    Display::printHeader("MODIFY PASSENGER");
    long long id = Input::getLLInput("Enter Passenger ID to modify: ");
    for (auto& p : Data::passengers) {
        if (p.id == id) {
            std::string v;
            v = Input::getInput("New FFC [" + p.frequent_flyer_code + "]: ");
            if (!v.empty()) p.frequent_flyer_code = v;
            v = Input::getInput("New Title [" + p.title + "]: ");
            if (!v.empty()) p.title = v;
            v = Input::getInput("New First Name [" + p.first_name + "]: ");
            if (!v.empty()) p.first_name = v;
            v = Input::getInput("New Last Name [" + p.last_name + "]: ");
            if (!v.empty()) p.last_name = v;
            v = Input::getInput("New Birthdate [" + p.birthdate + "]: ");
            if (!v.empty()) p.birthdate = v;
            v = Input::getInput("New Email [" + p.contact_email + "]: ");
            if (!v.empty()) p.contact_email = v;
            v = Input::getInput("New Emrg Name [" + p.emergency_contact_name +
                                "]: ");
            if (!v.empty()) p.emergency_contact_name = v;
            v = Input::getInput("New Emrg Email [" + p.emergency_contact_email +
                                "]: ");
            if (!v.empty()) p.emergency_contact_email = v;
            p.updated_at = "NOW()";
            API::Passenger::save();
            std::cout << "\n  [OK] Passenger updated.\n";
            return;
        }
    }
    std::cout << "\n  [!!] Passenger not found.\n";
}

void API::Passenger::remove() {
    Display::printHeader("DELETE PASSENGER");
    long long id = Input::getLLInput("Enter Passenger ID to delete: ");
    auto it = std::remove_if(
        Data::passengers.begin(), Data::passengers.end(),
        [id](const Structs::Passenger& p) { return p.id == id; });
    if (it != Data::passengers.end()) {
        Data::passengers.erase(it, Data::passengers.end());
        API::Passenger::save();
        std::cout << "\n  [OK] Passenger deleted.\n";
    } else
        std::cout << "\n  [!!] Passenger not found.\n";
}