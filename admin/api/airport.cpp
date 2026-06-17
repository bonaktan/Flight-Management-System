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
        {"ID", "Name", "Country", "City", "Capacity", "Created At"}};
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
        throw std::runtime_error("Airport with ID " + id + " not found");
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
        {"id", "name", "capacity", "country", "city"}};
    data.push_back({
        apiReturn["id"].get<std::string>(),
        apiReturn["name"].get<std::string>(),
        std::to_string(apiReturn["capacity"].get<int>()),
        apiReturn["country"].get<std::string>(),
        apiReturn["city"].get<std::string>(),
    });
    return data;
}

bool API::Airport::add(const std::map<std::string, std::string>& fields) {
    nlohmann::json airport(fields);
    static const std::unordered_set<std::string> intFields = {"capacity"};
    static const std::unordered_set<std::string> jsonFields = {};

    for (auto& [key, val] : fields) {
        if (intFields.count(key))
            airport[key] = std::stoi(val);
        else if (jsonFields.count(key))
            airport[key] = nlohmann::json::parse(val);
    }
    auto& client = API::ApiClient::getInstance();
    cpr::Response r = client.post("/admin/airport/add", airport);
    return r.status_code == 200 || r.status_code == 201;
}

bool API::Airport::modify(
    std::string id, std::map<std::string, std::string> fields) {
    API::ApiClient& client = API::ApiClient::getInstance();

    nlohmann::json payload = nlohmann::json::array();
    for (const auto& [field, value] : fields) {
        payload.push_back({{"field", field}, {"value", value}});
    }

    cpr::Response apiReturn =
        client.patch("/admin/airport/update/" + id, payload);

    if (apiReturn.status_code != 200) {
        return false;
    }
    return true;
}

bool API::Airport::remove(std::string id) {
    API::ApiClient& client = API::ApiClient::getInstance();
    cpr::Response apiReturn = client.del("/admin/airport/delete/" + id);
    return true;
}