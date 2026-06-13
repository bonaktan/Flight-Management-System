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
API::Airport* API::Airport::instance = nullptr;

std::vector<std::vector<std::string>> API::Airport::view() {
    API::ApiClient& client = API::ApiClient::getInstance();
    nlohmann::json apiReturn =
        nlohmann::json::parse(client.get("/admin/airport/view").text);
    std::vector<std::vector<std::string>> data = {
        {"ID", "Name", "Capacity", "Country", "City", "Created At"}};
    for (const auto& entry : apiReturn) {
        data.push_back({entry["id"].get<std::string>(),
                        entry["name"].get<std::string>(),
                        entry["country"].get<std::string>(),
                        entry["city"].get<std::string>(),
                        std::to_string(entry["capacity"].get<int>()),
                        entry["created_at"].get<std::string>()});
    }
    return data;
}

std::vector<std::vector<std::string>> API::Airport::view_one(std::string id) {
    API::ApiClient& client = API::ApiClient::getInstance();
    cpr::Response response = client.get("/admin/airport/view/" + id);

    if (response.status_code == 404)
        throw std::runtime_error("Booking with ID " + id + " not found");
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
        {"ID", "Name", "Capacity", "Country", "City"}};
    data.push_back({
        apiReturn["id"].get<std::string>(),
        apiReturn["name"].get<std::string>(),
        std::to_string(apiReturn["capacity"].get<int>()),
        apiReturn["country"].get<std::string>(),
        apiReturn["city"].get<std::string>(),
    });
    return data;
}

void API::Airport::add() {
    Display::printHeader("ADD AIRPORT");
    nlohmann::json airport;
    airport["name"] = Input::getInput(
        "Airport Name (e.g. Ninoy-Aquino International Airport): ");
    airport["airport_id"] = Input::getInput("Airport ID (e.g. MNL): ");
    airport["capacity"] = Input::getIntInput("Capacity (e.g. 50): ");
    airport["country"] = Input::getInput("Country (e.g. Philippines): ");
    airport["city"] = Input::getInput("City (e.g. Manila): ");

    API::ApiClient& client = API::ApiClient::getInstance();
    cpr::Response apiReturn = client.post("/admin/airport/add", airport);

    if (apiReturn.status_code == 201 || apiReturn.status_code == 200) {
        nlohmann::json response = nlohmann::json::parse(apiReturn.text);
        std::cout << "\n  [OK] Airport added.\n";
    } else {
        nlohmann::json errorResponse = nlohmann::json::parse(apiReturn.text);
        std::cerr << "\n  [ERROR] Failed to add airport: "
                  << errorResponse.value("message", "Unknown error") << "\n";
    }
}

std::vector<std::vector<std::string>> API::Airport::modify(std::string id,
                                                           std::string field,
                                                           std::string value) {
    API::ApiClient& client = API::ApiClient::getInstance();
    cpr::Response apiReturn =
        client.patch("/admin/airport/update/" + id,
                     nlohmann::json{{"field", field}, {"value", value}});
    if (apiReturn.status_code != 200) {
        throw std::runtime_error("Request failed with status: " +
                                 std::to_string(apiReturn.status_code));
    }

    std::vector<std::vector<std::string>> data = {
        {"ID", "Name", "Capacity", "Country", "City"}};
    nlohmann::json newData;

    try {
        newData = nlohmann::json::parse(apiReturn.text);
    } catch (const nlohmann::json::parse_error& e) {
        throw std::runtime_error(std::string("Failed to parse response: ") +
                                 e.what());
    }

    data.push_back({newData["id"].get<std::string>(),
                    newData["name"].get<std::string>(),
                    std::to_string(newData["capacity"].get<int>()),
                    newData["country"].get<std::string>(),
                    newData["city"].get<std::string>()});
    return data;
}

void API::Airport::remove() {
    Display::printHeader("DELETE AIRPORT");
    std::string id = Input::getInput("Enter Airport ID to delete: ");
    API::ApiClient& client = API::ApiClient::getInstance();
    cpr::Response apiReturn = client.del("/admin/airport/delete/" + id);
    std::cout << "\n  [OK] Airport deleted.\n";
}