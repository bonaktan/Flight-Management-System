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

void API::SeatClass::save() {
    std::ofstream f(Data::FILE_SEATCLASSES);
    for (auto& sc : Data::seatClasses)
        f << sc.id << "|" << Escape::escape(sc.class_name) << "|"
          << sc.amt_of_seats << "|" << std::fixed << std::setprecision(2)
          << sc.markup_price << "|" << Escape::escape(sc.airplane_id) << "|"
          << Escape::escape(sc.created_at) << "\n";
}

void API::SeatClass::load() {
    Data::seatClasses.clear();
    std::ifstream f(Data::FILE_SEATCLASSES);
    std::string line;
    while (std::getline(f, line)) {
        if (line.empty()) continue;
        auto t = Escape::splitLine(line);
        if (t.size() < 6) continue;
        Structs::SeatClass sc;
        sc.id = std::stoll(t[0]);
        sc.class_name = t[1];
        sc.amt_of_seats = std::stoi(t[2]);
        sc.markup_price = std::stod(t[3]);
        sc.airplane_id = t[4];
        sc.created_at = t[5];
        Data::seatClasses.push_back(sc);
    }
}

long long API::SeatClass::nextId() {
    long long mx = 0;
    for (auto& s : Data::seatClasses) mx = std::max(mx, s.id);
    return mx + 1;
}

void API::SeatClass::view() {
    Display::printHeader("SEAT CLASSES");
    if (Data::seatClasses.empty()) {
        std::cout << "  No records found.\n";
        return;
    }
    std::cout << "  " << std::left << std::setw(5) << "ID" << std::setw(20)
              << "Class Name" << std::setw(10) << "# Seats" << std::setw(14)
              << "Markup" << std::setw(15) << "Airplane ID" << "\n";
    Display::printDivider();
    for (auto& sc : Data::seatClasses)
        std::cout << "  " << std::setw(5) << sc.id << std::setw(20)
                  << sc.class_name << std::setw(10) << sc.amt_of_seats
                  << std::setw(14) << std::fixed << std::setprecision(2)
                  << sc.markup_price << std::setw(15) << sc.airplane_id << "\n";
}

void API::SeatClass::add() {
    Display::printHeader("ADD SEAT CLASS");
    Structs::SeatClass sc;
    sc.id = API::SeatClass::nextId();
    sc.class_name = Input::getInput("Class Name (e.g. Economy): ");
    sc.amt_of_seats = Input::getIntInput("Number of Seats: ");
    sc.markup_price = Input::getDoubleInput("Markup Price: ");
    sc.airplane_id = Input::getInput("Airplane ID: ");
    sc.created_at = "NOW()";
    Data::seatClasses.push_back(sc);
    API::SeatClass::save();
    std::cout << "\n  [OK] Seat Class added with ID " << sc.id << "\n";
}

void API::SeatClass::modify() {
    Display::printHeader("MODIFY SEAT CLASS");
    long long id = Input::getLLInput("Enter Seat Class ID to modify: ");
    for (auto& sc : Data::seatClasses) {
        if (sc.id == id) {
            std::string v;
            v = Input::getInput("New Class Name [" + sc.class_name + "]: ");
            if (!v.empty()) sc.class_name = v;
            std::string s = Input::getInput(
                "New # Seats [" + std::to_string(sc.amt_of_seats) + "]: ");
            if (!s.empty()) sc.amt_of_seats = std::stoi(s);
            std::string m = Input::getInput(
                "New Markup Price [" + std::to_string(sc.markup_price) + "]: ");
            if (!m.empty()) sc.markup_price = std::stod(m);
            v = Input::getInput("New Airplane ID [" + sc.airplane_id + "]: ");
            if (!v.empty()) sc.airplane_id = v;
            API::SeatClass::save();
            std::cout << "\n  [OK] Seat Class updated.\n";
            return;
        }
    }
    std::cout << "\n  [!!] Seat Class not found.\n";
}

void API::SeatClass::remove() {
    Display::printHeader("DELETE SEAT CLASS");
    long long id = Input::getLLInput("Enter Seat Class ID to delete: ");
    auto it = std::remove_if(
        Data::seatClasses.begin(), Data::seatClasses.end(),
        [id](const Structs::SeatClass& sc) { return sc.id == id; });
    if (it != Data::seatClasses.end()) {
        Data::seatClasses.erase(it, Data::seatClasses.end());
        API::SeatClass::save();
        std::cout << "\n  [OK] Seat Class deleted.\n";
    } else
        std::cout << "\n  [!!] Seat Class not found.\n";
}