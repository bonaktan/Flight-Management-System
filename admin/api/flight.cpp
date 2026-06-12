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
API::Flight* API::Flight::instance = nullptr;

void API::Flight::add() {
    Display::printHeader("ADD FLIGHT");
    nlohmann::json flight;
    flight["flight_id"] = Input::getInput("Flight ID (e.g. SKY421): ");
    flight["departure_airport_id"] =
        Input::getInput("Departure Airport ID (e.g. MNL): ");
    flight["arrival_airport_id"] =
        Input::getInput("Arrival Airport ID (e.g. CEB): ");
    flight["flight_time"] = Input::getInput("Flight Time (e.g. 01:30:00): ");
    flight["base_ticket_price"] =
        Input::getIntInput("Base Ticket Price (e.g. 4000): ");
    flight["start_of_operations"] = Input::getInput(
        "Start of Operations (e.g. 2026-06-05T15:00:00+08:00): ");
    flight["frequency"] = Input::getInput("Frequency (e.g. P1D): ");
    flight["airplane"] = Input::getInput("Airplane ID (e.g. SB-W0001): ");

    API::ApiClient& client = API::ApiClient::getInstance();
    cpr::Response apiReturn = client.post("/admin/flight/add", flight);

    if (apiReturn.status_code == 201 || apiReturn.status_code == 200) {
        nlohmann::json response = nlohmann::json::parse(apiReturn.text);
        std::cout << "\n  [OK] Flight added.\n";
    } else {
        nlohmann::json errorResponse = nlohmann::json::parse(apiReturn.text);
        std::cerr << "\n  [ERROR] Failed to add flight: "
                  << errorResponse.value("message", "Unknown error") << "\n";
    }
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
            if (!pr.empty()) fl.base_ticket_price = std::stod(pr);
            v = Input::getInput("New Flight Time [" + fl.flight_time + "]: ");
            if (!v.empty()) fl.flight_time = v;
            v = Input::getInput("New Departure [" + fl.departure + "]: ");
            if (!v.empty()) fl.departure = v;
            v = Input::getInput("New Frequency [" + fl.frequency + "]: ");
            if (!v.empty()) fl.frequency = v;
            std::cout << "\n  [OK] Flight updated.\n";
            return;
        }
    }
    std::cout << "\n  [!!] Flight not found.\n";
}

void API::Flight::remove() {
    Display::printHeader("DELETE FLIGHT");
    std::string id = Input::getInput("Enter Flight ID to delete: ");
    API::ApiClient& client = API::ApiClient::getInstance();
    cpr::Response apiReturn = client.del("/admin/flight/delete/" + id);
    // TODO: error handling
    std::cout << "\n  [OK] Flight deleted.\n";
}