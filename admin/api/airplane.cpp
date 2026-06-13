#include <algorithm>
#include <fstream>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/screen.hpp>
#include <iomanip>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include "../controls/controls.h"
#include "../display/display.h"
#include "./api.h"

using namespace Skybridge;
API::Airplane* API::Airplane::instance = nullptr;

std::vector<std::vector<std::string>> API::Airplane::view() {
    API::ApiClient& client = API::ApiClient::getInstance();
    nlohmann::json apiReturn =
        nlohmann::json::parse(client.get("/admin/airplane/view").text);

    std::vector<std::vector<std::string>> data = {
        {"ID", "Location", "Model", "Seatmap", "Seat Class"}};
    for (const auto& entry : apiReturn) {
        data.push_back({entry["id"].get<std::string>(),
                        entry["location"].get<std::string>(),
                        entry["model"].get<std::string>(),
                        entry["seatmap"].dump(), entry["seat_class"].dump()});
    }
    return data;
}

std::vector<std::vector<std::string>> API::Airplane::view_one(std::string id) {
    API::ApiClient& client = API::ApiClient::getInstance();
    cpr::Response response = client.get("/admin/airplane/view/" + id);

    if (response.status_code == 404)
        throw std::runtime_error("Airplane with ID " + id + " not found");
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
        {"id", "location", "model", "seatmap", "seat_class"}};
    data.push_back({apiReturn["id"].get<std::string>(),
                    apiReturn["location"].get<std::string>(),
                    apiReturn["model"].get<std::string>(),
                    apiReturn["seatmap"].dump(),
                    apiReturn["seat_class"].dump()});
    return data;
}

void API::Airplane::add() {
    Display::printHeader("ADD AIRPLANE");
    nlohmann::json airplane;
    airplane["airplane_id"] = Input::getInput("Airplane ID (e.g. RP-C8888): ");
    airplane["model"] = Input::getInput("Model (e.g. Boeing 737): ");
    airplane["location"] = Input::getInput("Location (Airport ID): ");

    API::ApiClient& client = API::ApiClient::getInstance();
    cpr::Response apiReturn = client.post("/admin/airplane/add", airplane);
    if (apiReturn.status_code == 201 || apiReturn.status_code == 200) {
        nlohmann::json response = nlohmann::json::parse(apiReturn.text);
        std::cout << "\n  [OK] Airplane added.\n";
    } else {
        nlohmann::json errorResponse = nlohmann::json::parse(apiReturn.text);
        std::cerr << "\n  [ERROR] Failed to add airplane: "
                  << errorResponse.value("message", "Unknown error") << "\n";
    }
    std::cout << "\n  [OK] Airplane added.\n";
}

std::vector<std::vector<std::string>> API::Airplane::modify(std::string id,
                                                            std::string field,
                                                            std::string value) {
    API::ApiClient& client = API::ApiClient::getInstance();
    cpr::Response apiReturn =
        client.patch("/admin/airplane/update/" + id,
                     nlohmann::json{{"field", field}, {"value", value}});
    if (apiReturn.status_code != 200) {
        throw std::runtime_error("Request failed with status: " +
                                 std::to_string(apiReturn.status_code));
    }

    std::vector<std::vector<std::string>> data = {
        {"ID", "Model", "Location", "Seatmap", "Seat Class"}};
    nlohmann::json newData;
    try {
        newData = nlohmann::json::parse(apiReturn.text);
    } catch (const nlohmann::json::parse_error& e) {
        throw std::runtime_error(std::string("Failed to parse response: ") +
                                 e.what());
    }

    data.push_back({newData["id"].get<std::string>(),
                    newData["model"].get<std::string>(),
                    newData["location"].get<std::string>(),
                    newData["seatmap"].dump(), newData["seat_class"].dump()});
    return data;
}
void API::Airplane::remove() {
    Display::printHeader("DELETE AIRPLANE");
    std::string id = Input::getInput("Enter Airplane ID to delete: ");
    API::ApiClient& client = API::ApiClient::getInstance();
    cpr::Response apiReturn = client.del("/admin/airplane/delete/" + id);
    std::cout << "\n  [OK] Airplane deleted.\n";
}