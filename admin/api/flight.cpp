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

void API::Flight::save() {
    std::ofstream f(Data::FILE_FLIGHTS);
    for (auto& fl : Data::flights)
        f << Escape::escape(fl.id) << "|"
          << Escape::escape(fl.departure_airport_id) << "|"
          << Escape::escape(fl.arrival_airport_id) << "|" << std::fixed
          << std::setprecision(2) << fl.base_ticket_price << "|"
          << Escape::escape(fl.flight_time) << "|"
          << Escape::escape(fl.departure) << "|" << Escape::escape(fl.frequency)
          << "|" << Escape::escape(fl.created_at) << "\n";
}

void API::Flight::load() {
    Data::flights.clear();
    std::ifstream f(Data::FILE_FLIGHTS);
    std::string line;
    while (std::getline(f, line)) {
        if (line.empty()) continue;
        auto t = Escape::splitLine(line);
        if (t.size() < 8) continue;
        Structs::Flight fl;
        try {
            fl.id = t[0];
            fl.departure_airport_id = t[1];
            fl.arrival_airport_id = t[2];
            fl.base_ticket_price = Input::getDoubleInput(t[3]);
            fl.flight_time = t[4];
            fl.departure = t[5];
            fl.frequency = t[6];
            fl.created_at = t[7];
            Data::flights.push_back(fl);
        } catch (std::invalid_argument) {
            std::cerr << "Skipping Flight: Input - Not a Number" << std::endl;
        } catch (std::out_of_range) {
            std::cerr << "Skipping Flight: Input - Too Large: " << std::endl;
        }
    }
}

void API::Flight::view() {
    Display::printHeader("FLIGHTS");
    if (Data::flights.empty()) {
        std::cout << "  No records found.\n";
        return;
    }
    std::cout << "  " << std::left << std::setw(12) << "ID" << std::setw(10)
              << "Dep.AP" << std::setw(10) << "Arr.AP" << std::setw(12)
              << "Price" << std::setw(12) << "Duration" << std::setw(22)
              << "Departure" << std::setw(12) << "Frequency" << "\n";
    Display::printDivider();
    for (auto& fl : Data::flights)
        std::cout << "  " << std::setw(12) << fl.id << std::setw(10)
                  << fl.departure_airport_id << std::setw(10)
                  << fl.arrival_airport_id << std::setw(12) << std::fixed
                  << std::setprecision(2) << fl.base_ticket_price
                  << std::setw(12) << fl.flight_time << std::setw(22)
                  << fl.departure << std::setw(12) << fl.frequency << "\n";
}

void API::Flight::add() {
    Display::printHeader("ADD FLIGHT");
    Structs::Flight fl;
    fl.id = Input::getInput("Flight ID (e.g. PR101): ");
    fl.departure_airport_id = Input::getInput("Departure Airport ID: ");
    fl.arrival_airport_id = Input::getInput("Arrival Airport ID: ");
    fl.base_ticket_price = Input::getDoubleInput("Base Ticket Price: ");
    fl.flight_time = Input::getInput("Flight Time (e.g. 02:30:00): ");
    fl.departure = Input::getInput("Departure (YYYY-MM-DD HH:MM:SS): ");
    fl.frequency = Input::getInput("Frequency (e.g. 7 days): ");
    fl.created_at = "NOW()";
    Data::flights.push_back(fl);
    save();
    std::cout << "\n  [OK] Flight added.\n";
}

void API::Flight::modify() {
    Display::printHeader("MODIFY FLIGHT");
    std::string id = Input::getInput("Enter Flight ID to modify: ");
    for (auto& fl : Data::flights) {
        if (fl.id == id) {
            std::string v;
            v = Input::getInput("New Dep Airport [" + fl.departure_airport_id +
                                "]: ");
            if (!v.empty()) fl.departure_airport_id = v;
            v = Input::getInput("New Arr Airport [" + fl.arrival_airport_id +
                                "]: ");
            if (!v.empty()) fl.arrival_airport_id = v;
            std::string pr = Input::getInput(
                "New Price [" + std::to_string(fl.base_ticket_price) + "]: ");
            if (!pr.empty()) fl.base_ticket_price = Input::getDoubleInput(pr);
            v = Input::getInput("New Flight Time [" + fl.flight_time + "]: ");
            if (!v.empty()) fl.flight_time = v;
            v = Input::getInput("New Departure [" + fl.departure + "]: ");
            if (!v.empty()) fl.departure = v;
            v = Input::getInput("New Frequency [" + fl.frequency + "]: ");
            if (!v.empty()) fl.frequency = v;
            API::Flight::save();
            std::cout << "\n  [OK] Flight updated.\n";
            return;
        }
    }
    std::cout << "\n  [!!] Flight not found.\n";
}

void API::Flight::remove() {
    Display::printHeader("DELETE FLIGHT");
    std::string id = Input::getInput("Enter Flight ID to delete: ");
    auto it = std::remove_if(
        Data::flights.begin(), Data::flights.end(),
        [&id](const Structs::Flight& fl) { return fl.id == id; });
    if (it != Data::flights.end()) {
        Data::flights.erase(it, Data::flights.end());
        API::Flight::save();
        std::cout << "\n  [OK] Flight deleted.\n";
    } else
        std::cout << "\n  [!!] Flight not found.\n";
}