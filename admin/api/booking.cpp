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

void API::Booking::save() {
    std::ofstream f(Data::FILE_BOOKINGS);
    for (auto& b : Data::bookings)
        f << b.id << "|" << b.passenger_id << "|" << Escape::escape(b.flight_id)
          << "|" << b.account_id << "|" << std::fixed << std::setprecision(2)
          << b.calculated_price << "|" << Escape::escape(b.payment_option)
          << "|" << Escape::escape(b.payment_detail) << "|"
          << Escape::escape(Display::bookingStatusToStr(b.booking_status))
          << "|" << Escape::escape(b.created_at) << "|"
          << Escape::escape(b.updated_at) << "\n";
}

void API::Booking::load() {
    Data::bookings.clear();
    std::ifstream f(Data::FILE_BOOKINGS);
    std::string line;
    while (std::getline(f, line)) {
        if (line.empty()) continue;
        auto t = Escape::splitLine(line);
        if (t.size() < 10) continue;
        Structs::Booking b;
        b.id = std::stoll(t[0]);
        b.passenger_id = std::stoll(t[1]);
        b.flight_id = t[2];
        b.account_id = std::stoll(t[3]);
        b.calculated_price = std::stod(t[4]);
        b.payment_option = t[5];
        b.payment_detail = t[6];
        b.booking_status = Display::strToBookingStatus(t[7]);
        b.created_at = t[8];
        b.updated_at = t[9];
        Data::bookings.push_back(b);
    }
}

long long API::Booking::nextId() {
    long long mx = 0;
    for (auto& b : Data::bookings) mx = std::max(mx, b.id);
    return mx + 1;
}

void API::Booking::view() {
    Display::printHeader("BOOKINGS");
    if (Data::bookings.empty()) {
        std::cout << "  No records found.\n";
        return;
    }
    std::cout << "  " << std::left << std::setw(5) << "ID" << std::setw(11)
              << "Passenger" << std::setw(12) << "Flight" << std::setw(9)
              << "Account" << std::setw(12) << "Price" << std::setw(12)
              << "Payment" << std::setw(12) << "Status" << "\n";
    Display::printDivider();
    for (auto& b : Data::bookings)
        std::cout << "  " << std::setw(5) << b.id << std::setw(11)
                  << b.passenger_id << std::setw(12) << b.flight_id
                  << std::setw(9)
                  << (b.account_id == -1 ? "N/A" : std::to_string(b.account_id))
                  << std::setw(12) << std::fixed << std::setprecision(2)
                  << b.calculated_price << std::setw(12) << b.payment_option
                  << std::setw(12)
                  << Display::bookingStatusToStr(b.booking_status) << "\n";
}

void API::Booking::add() {
    Display::printHeader("ADD BOOKING");
    Structs::Booking b;
    b.id = API::Booking::nextId();
    b.passenger_id = Input::getLLInput("Passenger ID: ");
    b.flight_id = Input::getInput("Flight ID: ");
    std::string ac = Input::getInput("Account ID (blank if none): ");
    b.account_id = ac.empty() ? -1 : std::stoll(ac);
    b.calculated_price = Input::getDoubleInput("Calculated Price: ");
    b.payment_option = Input::getInput("Payment Option (cash/card/etc): ");
    b.payment_detail =
        Input::getInput("Payment Detail (JSON string, e.g. {}): ");
    std::cout << "  Status [1=pending, 2=confirmed, 3=cancelled]: ";
    std::string sc;
    std::getline(std::cin, sc);
    if (sc == "2")
        b.booking_status = Structs::BookingStatus::CONFIRMED;
    else if (sc == "3")
        b.booking_status = Structs::BookingStatus::CANCELLED;
    else
        b.booking_status = Structs::BookingStatus::PENDING;
    b.created_at = b.updated_at = "NOW()";
    Data::bookings.push_back(b);
    API::Booking::save();
    std::cout << "\n  [OK] Booking added with ID " << b.id << "\n";
}

void API::Booking::modify() {
    Display::printHeader("MODIFY BOOKING");
    long long id = Input::getLLInput("Enter Booking ID to modify: ");
    for (auto& b : Data::bookings) {
        if (b.id == id) {
            std::string v;
            v = Input::getInput("New Payment Option [" + b.payment_option +
                                "]: ");
            if (!v.empty()) b.payment_option = v;
            v = Input::getInput("New Payment Detail [" + b.payment_detail +
                                "]: ");
            if (!v.empty()) b.payment_detail = v;
            std::cout << "  New Status [1=pending, 2=confirmed, 3=cancelled] "
                         "(current: "
                      << Display::bookingStatusToStr(b.booking_status) << "): ";
            std::string sc;
            std::getline(std::cin, sc);
            if (sc == "1")
                b.booking_status = Structs::BookingStatus::PENDING;
            else if (sc == "2")
                b.booking_status = Structs::BookingStatus::CONFIRMED;
            else if (sc == "3")
                b.booking_status = Structs::BookingStatus::CANCELLED;
            b.updated_at = "NOW()";
            API::Booking::save();
            std::cout << "\n  [OK] Booking updated.\n";
            return;
        }
    }
    std::cout << "\n  [!!] Booking not found.\n";
}

void API::Booking::remove() {
    Display::printHeader("DELETE BOOKING");
    long long id = Input::getLLInput("Enter Booking ID to delete: ");
    auto it =
        std::remove_if(Data::bookings.begin(), Data::bookings.end(),
                       [id](const Structs::Booking& b) { return b.id == id; });
    if (it != Data::bookings.end()) {
        Data::bookings.erase(it, Data::bookings.end());
        save();
        std::cout << "\n  [OK] Booking deleted.\n";
    } else
        std::cout << "\n  [!!] Booking not found.\n";
}