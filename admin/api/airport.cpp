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
        {"ID", "Name", "Capacity", "Created At"}};
    for (const auto& entry : apiReturn) {
        data.push_back({entry["id"].get<std::string>(),
                        entry["name"].get<std::string>(),
                        std::to_string(entry["capacity"].get<int>()),
                        entry["created_at"].get<std::string>()});
    }
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
            if (!c.empty()) a.capacity = std::stoi(c);
            std::cout << "\n  [OK] Airport updated.\n";
            return;
        }
    }
    std::cout << "\n  [!!] Airport not found.\n";
}

void API::Airport::remove() {
    Display::printHeader("DELETE AIRPORT");
    std::string id = Input::getInput("Enter Airport ID to delete: ");
    API::ApiClient& client = API::ApiClient::getInstance();
    cpr::Response apiReturn = client.del("/admin/airport/delete/" + id);
        std::cout << "\n  [OK] Airport deleted.\n";
}