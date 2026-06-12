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
API::Passenger* API::Passenger::instance = nullptr;

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
        std::cout << "\n  [OK] Passenger deleted.\n";
    } else
        std::cout << "\n  [!!] Passenger not found.\n";
}