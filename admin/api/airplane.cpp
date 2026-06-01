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

void API::Airplane::save() {
    std::ofstream f(Data::FILE_AIRPLANES);
    for (auto& a : Data::airplanes)
        f << Escape::escape(a.id) << "|" << Escape::escape(a.model) << "|"
          << Escape::escape(a.location) << "\n";
}

void API::Airplane::load() {
    Data::airplanes.clear();
    std::ifstream f(Data::FILE_AIRPLANES);
    std::string line;
    while (std::getline(f, line)) {
        if (line.empty()) continue;
        auto t = Escape::splitLine(line);
        if (t.size() < 3) continue;
        Structs::Airplane a;
        a.id = t[0];
        a.model = t[1];
        a.location = t[2];
        Data::airplanes.push_back(a);
    }
}

void API::Airplane::view() {
    Display::printHeader("AIRPLANES");
    if (Data::airplanes.empty()) {
        std::cout << "  No records found.\n";
        return;
    }
    std::cout << "  " << std::left << std::setw(15) << "ID" << std::setw(28)
              << "Model" << std::setw(12) << "Location" << "\n";
    Display::printDivider();
    for (auto& a : Data::airplanes)
        std::cout << "  " << std::setw(15) << a.id << std::setw(28) << a.model
                  << std::setw(12) << a.location << "\n";
}

void API::Airplane::add() {
    Display::printHeader("ADD AIRPLANE");
    Structs::Airplane a;
    a.id = Input::getInput("Airplane ID (e.g. RP-C8888): ");
    a.model = Input::getInput("Model (e.g. Boeing 737): ");
    a.location = Input::getInput("Location (Airport ID, or blank): ");
    Data::airplanes.push_back(a);
    API::Airplane::save();
    std::cout << "\n  [OK] Airplane added.\n";
}

void API::Airplane::modify() {
    Display::printHeader("MODIFY AIRPLANE");
    std::string id = Input::getInput("Enter Airplane ID to modify: ");
    for (auto& a : Data::airplanes) {
        if (a.id == id) {
            std::string v;
            v = Input::getInput("New Model [" + a.model + "]: ");
            if (!v.empty()) a.model = v;
            v = Input::getInput("New Location [" + a.location + "]: ");
            if (!v.empty()) a.location = v;
            API::Airplane::save();
            std::cout << "\n  [OK] Airplane updated.\n";
            return;
        }
    }
    std::cout << "\n  [!!] Airplane not found.\n";
}

void API::Airplane::remove() {
    Display::printHeader("DELETE AIRPLANE");
    std::string id = Input::getInput("Enter Airplane ID to delete: ");
    auto it = std::remove_if(
        Data::airplanes.begin(), Data::airplanes.end(),
        [&id](const Structs::Airplane& a) { return a.id == id; });
    if (it != Data::airplanes.end()) {
        Data::airplanes.erase(it, Data::airplanes.end());
        API::Airplane::save();
        std::cout << "\n  [OK] Airplane deleted.\n";
    } else
        std::cout << "\n  [!!] Airplane not found.\n";
}