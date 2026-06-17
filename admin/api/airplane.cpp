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

bool API::Airplane::add(const std::map<std::string, std::string>& fields) {
    nlohmann::json airplane(fields);
    static const std::unordered_set<std::string> jsonFields = {"seatmap",
                                                               "seat_class"};

    for (auto& [key, val] : fields) {
        if (jsonFields.count(key)) airplane[key] = nlohmann::json::parse(val);
    }
    auto& client = API::ApiClient::getInstance();
    cpr::Response r = client.post("/admin/airplane/add", airplane);
    return r.status_code == 200 || r.status_code == 201;
}

bool API::Airplane::modify(
    std::string id, std::map<std::string, std::string> fields) {
    API::ApiClient& client = API::ApiClient::getInstance();

    nlohmann::json payload = nlohmann::json::array();
    for (const auto& [field, value] : fields) {
        payload.push_back({{"field", field}, {"value", value}});
    }

    cpr::Response apiReturn =
        client.patch("/admin/airplane/update/" + id, payload);

    if (apiReturn.status_code != 200) {
        return false;
    }
    return true;
}

bool API::Airplane::remove(std::string id) {
    API::ApiClient& client = API::ApiClient::getInstance();
    cpr::Response apiReturn = client.del("/admin/airplane/delete/" + id);
    return true;
}