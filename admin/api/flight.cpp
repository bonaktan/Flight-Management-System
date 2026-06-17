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

std::vector<std::vector<std::string>> API::Flight::view() {
    API::ApiClient& client = API::ApiClient::getInstance();
    nlohmann::json apiReturn =
        nlohmann::json::parse(client.get("/admin/flight/view").text);
    std::vector<std::vector<std::string>> data = {
        {"ID", "Departure Airport", "Arrival Airport", "Base Price",
         "Flight Time", "Departure", "Frequency", "Created At", "Airplane ID"}};
    for (const auto& entry : apiReturn) {
        data.push_back(
            {entry["id"].get<std::string>(),
             entry["departure_airport_id"].get<std::string>(),
             entry["arrival_airport_id"].get<std::string>(),
             std::to_string(entry["base_ticket_price"].get<double>()),
             entry["flight_time"].get<std::string>(),
             entry["departure"].get<std::string>(),
             entry["frequency"].get<std::string>(),
             entry["created_at"].get<std::string>(),
             entry["airplane_id"].get<std::string>()});
    }
    return data;
}

std::vector<std::vector<std::string>> API::Flight::view_one(std::string id) {
    API::ApiClient& client = API::ApiClient::getInstance();
    cpr::Response response = client.get("/admin/flight/view/" + id);

    if (response.status_code == 404)
        throw std::runtime_error("Account with ID " + id + " not found");
    if (response.status_code != 200)
        throw std::runtime_error("Request failed with status: " +
                                 std::to_string(response.status_code));

    nlohmann::json apiReturn;
    try {
        apiReturn = nlohmann::json::parse(response.text);
    } catch (const nlohmann::json::parse_error& e) {
        throw std::runtime_error(std::string("Failed to parse response: ") +
                                 e.what());
    }
    std::vector<std::vector<std::string>> data = {
        {"id", "departure_airport_id", "arrival_airport_id",
         "base_ticket_price", "flight_time", "departure", "frequency",
         "created_at", "airplane_id"}};
    data.push_back(
        {apiReturn["id"].get<std::string>(),
         apiReturn["departure_airport_id"].get<std::string>(),
         apiReturn["arrival_airport_id"].get<std::string>(),
         std::to_string(apiReturn["base_ticket_price"].get<double>()),
         apiReturn["flight_time"].get<std::string>(),
         apiReturn["departure"].get<std::string>(),
         apiReturn["frequency"].get<std::string>(),
         apiReturn["created_at"].get<std::string>(),
         apiReturn["airplane_id"].get<std::string>()});
    return data;
}

bool API::Flight::add(const std::map<std::string, std::string>& fields) {
    nlohmann::json flight(fields);
    static const std::unordered_set<std::string> intFields = {"account_id"};
    static const std::unordered_set<std::string> jsonFields = {"base_ticket_price"};

    for (auto& [key, val] : fields) {
        if (intFields.count(key))
            flight[key] = std::stoi(val);
        else if (jsonFields.count(key))
            flight[key] = nlohmann::json::parse(val);
    }

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
    return false;
}

std::vector<std::vector<std::string>> API::Flight::modify(std::string id,
                                                          std::string field,
                                                          std::string value) {
    API::ApiClient& client = API::ApiClient::getInstance();
    cpr::Response apiReturn =
        client.patch("/admin/flight/update/" + id,
                     nlohmann::json{{"field", field}, {"value", value}});
    if (apiReturn.status_code != 200) {
        throw std::runtime_error("Request failed with status: " +
                                 std::to_string(apiReturn.status_code));
    }

    std::vector<std::vector<std::string>> data = {
        {"ID", "Departure Airport", "Arrival Airport", "Base Ticket Price",
         "Flight Time", "Departure", "Frequency", "Created At", "Airplane ID"}};
    nlohmann::json newData;
    try {
        newData = nlohmann::json::parse(apiReturn.text);
    } catch (const nlohmann::json::parse_error& e) {
        throw std::runtime_error(std::string("Failed to parse response: ") +
                                 e.what());
    }

    data.push_back({newData["id"].get<std::string>(),
                    newData["departure_airport_id"].get<std::string>(),
                    newData["arrival_airport_id"].get<std::string>(),
                    std::to_string(newData["base_ticket_price"].get<double>()),
                    newData["flight_time"].get<std::string>(),
                    newData["departure"].get<std::string>(),
                    newData["frequency"].get<std::string>(),
                    newData["created_at"].get<std::string>(),
                    newData["airplane_id"].get<std::string>()});
    return data;
}

void API::Flight::remove() {
    Display::printHeader("DELETE FLIGHT");
    std::string id = Input::getInput("Enter Flight ID to delete: ");
    API::ApiClient& client = API::ApiClient::getInstance();
    cpr::Response apiReturn = client.del("/admin/flight/delete/" + id);
    // TODO: error handling
    std::cout << "\n  [OK] Flight deleted.\n";
}