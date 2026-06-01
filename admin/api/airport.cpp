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

void API::Airport::save() {
    std::ofstream f(Data::FILE_AIRPORTS);
    for (auto& a : Data::airports)
        f << Escape::escape(a.id) << "|" << Escape::escape(a.name) << "|"
          << a.capacity << "|" << Escape::escape(a.created_at) << "\n";
}

void API::Airport::load() {
    Data::airports.clear();
    std::ifstream f(Data::FILE_AIRPORTS);
    std::string line;
    while (std::getline(f, line)) {
        if (line.empty()) continue;
        auto t = Escape::splitLine(line);
        if (t.size() < 4) continue;
        Structs::Airport a;
        try {
            a.id = t[0];
            a.name = t[1];
            a.capacity = Input::getIntInput(t[2]);
            a.created_at = t[3];
            Data::airports.push_back(a);
        } catch (std::invalid_argument) {
            std::cerr << "Skipping Airport: Input - Not a Number" << std::endl;
        } catch (std::out_of_range) {
            std::cerr << "Skipping Airport: Input - Too Large: " << std::endl;
        }
    }
}

void API::Airport::view() {
    Display::printHeader("AIRPORTS");
    if (Data::airports.empty()) {
        std::cout << "  No records found.\n";
        return;
    }
    std::cout << "  " << std::left << std::setw(12) << "ID" << std::setw(32)
              << "Name" << std::setw(10) << "Capacity" << "\n";
    Display::printDivider();
    for (auto& a : Data::airports)
        std::cout << "  " << std::setw(12) << a.id << std::setw(32) << a.name
                  << std::setw(10) << a.capacity << "\n";
}

void API::Airport::add() {
    Display::printHeader("ADD AIRPORT");
    Structs::Airport a;
    a.id = Input::getInput("Airport ID (e.g. MNL): ");
    a.name = Input::getInput("Airport Name: ");
    a.capacity = Input::getIntInput("Capacity: ");
    a.created_at = "NOW()";
    Data::airports.push_back(a);
    API::Airport::save();
    std::cout << "\n  [OK] Airport added.\n";
}

void API::Airport::modify() {
    Display::printHeader("MODIFY AIRPORT");
    std::string id = Input::getInput("Enter Airport ID to modify: ");
    for (auto& a : Data::airports) {
        if (a.id == id) {
            std::string v;
            v = Input::getInput("New Name [" + a.name + "]: ");
            if (!v.empty()) a.name = v;
            std::string c = Input::getInput("New Capacity [" +
                                            std::to_string(a.capacity) + "]: ");
            if (!c.empty()) {
                try {
                    a.capacity = Input::getIntInput(c);
                } catch (std::invalid_argument) {
                    std::cerr << "ERROR: Invalid Input - Not a Number"
                              << std::endl;
                    return;
                } catch (std::out_of_range) {
                    std::cerr << "ERROR: Invalid Input - Too Large: "
                              << std::endl;
                    
                }
            }
            API::Airport::save();
            std::cout << "\n  [OK] Airport updated.\n";
            return;
        }
    }
    std::cout << "\n  [!!] Airport not found.\n";
}

void API::Airport::remove() {
    Display::printHeader("DELETE AIRPORT");
    std::string id = Input::getInput("Enter Airport ID to delete: ");
    auto it =
        std::remove_if(Data::airports.begin(), Data::airports.end(),
                       [&id](const Structs::Airport& a) { return a.id == id; });
    if (it != Data::airports.end()) {
        Data::airports.erase(it, Data::airports.end());
        API::Airport::save();
        std::cout << "\n  [OK] Airport deleted.\n";
    } else
        std::cout << "\n  [!!] Airport not found.\n";
}