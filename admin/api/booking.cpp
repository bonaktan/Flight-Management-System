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
API::Booking* API::Booking::instance = nullptr;

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
    std::cout << "\n  [OK] Booking added with ID " << b.id << "\n";
}

std::vector<std::vector<std::string>> API::Booking::modify(std::string id,
                                                            std::string field,
                                                            std::string value) {
    API::ApiClient& client = API::ApiClient::getInstance();
    cpr::Response apiReturn = client.patch(
        "/admin/booking/update/" + id, nlohmann::json{{"field", field}, {"value", value}});
    std::vector<std::vector<std::string>> data = {
        {"ID", "Flight ID", "Account ID", "Payment Option", "Payment Detail",
         "Booking Status", "Created At", "Updated At", "Departure Date"}};
    nlohmann::json newData = nlohmann::json::parse(apiReturn.text);
    data.push_back({std::to_string(newData["id"].get<long long>()),
                    newData["flight_id"].get<std::string>(),
                    std::to_string(newData["account_id"].get<long long>()),
                    newData["payment_option"].get<std::string>(),
                    newData["payment_detail"].dump(),
                    newData["booking_status"].get<std::string>(),
                    newData["created_at"].get<std::string>(),
                    newData["updated_at"].get<std::string>(),
                    newData["departure_date"].get<std::string>()});
    return data;
}

void API::Booking::remove() {
    Display::printHeader("DELETE BOOKING");
    long long id = Input::getLLInput("Enter Booking ID to delete: ");
    auto it =
        std::remove_if(Data::bookings.begin(), Data::bookings.end(),
                       [id](const Structs::Booking& b) { return b.id == id; });
    if (it != Data::bookings.end()) {
        Data::bookings.erase(it, Data::bookings.end());
        std::cout << "\n  [OK] Booking deleted.\n";
    } else
        std::cout << "\n  [!!] Booking not found.\n";
}