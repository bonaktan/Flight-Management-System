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

std::vector<std::vector<std::string>> API::Booking::view() {
    API::ApiClient& client = API::ApiClient::getInstance();
    nlohmann::json apiReturn =
        nlohmann::json::parse(client.get("/admin/booking/view").text);
    std::vector<std::vector<std::string>> data = {
        {"ID", "Flight ID", "Account ID", "Payment Option", "Payment Detail",
         "Status", "Departure Date", "Created At", "Updated At"}};
    for (const auto& entry : apiReturn) {
        data.push_back({std::to_string(entry["id"].get<long long>()),
                        entry["flight_id"].get<std::string>(),
                        std::to_string(entry["account_id"].get<long long>()),
                        entry["payment_option"].get<std::string>(),
                        entry["payment_detail"].dump(),
                        entry["booking_status"].get<std::string>(),
                        entry["departure_date"].get<std::string>(),
                        entry["created_at"].get<std::string>(),
                        entry["updated_at"].get<std::string>()});
    }
    return data;
}

std::vector<std::vector<std::string>> API::Booking::view_one(std::string id) {
    API::ApiClient& client = API::ApiClient::getInstance();
    cpr::Response response = client.get("/admin/booking/view/" + id);

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
        {"id", "flight_id", "account_id", "payment_option", "payment_detail",
         "booking_status", "departure_date", "created_at", "updated_at"}};
    data.push_back({std::to_string(apiReturn["id"].get<long long>()),
                    apiReturn["flight_id"].get<std::string>(),
                    std::to_string(apiReturn["account_id"].get<long long>()),
                    apiReturn["payment_option"].get<std::string>(),
                    apiReturn["payment_detail"].dump(),
                    apiReturn["booking_status"].get<std::string>(),
                    apiReturn["departure_date"].get<std::string>(),
                    apiReturn["created_at"].get<std::string>(),
                    apiReturn["updated_at"].get<std::string>()});
    return data;
}

bool API::Booking::add(const std::map<std::string, std::string>& fields) {
    nlohmann::json booking(fields);
    static const std::unordered_set<std::string> intFields = {"account_id"};
    static const std::unordered_set<std::string> jsonFields = {"payment_detail"};

    for (auto& [key, val] : fields) {
        if (intFields.count(key))
            booking[key] = std::stoi(val);
        else if (jsonFields.count(key))
            booking[key] = nlohmann::json::parse(val);
    }
    API::ApiClient& client = API::ApiClient::getInstance();
    cpr::Response apiReturn = client.post("/admin/booking/add", booking);
    if (apiReturn.status_code == 200 || apiReturn.status_code == 201) {
        std::cout << "\n  [OK] Booking added.\n";
    } else {
        nlohmann::json errorResponse = nlohmann::json::parse(apiReturn.text);
        std::cerr << "\n  [ERROR] Failed to add booking: "
                  << errorResponse.value("message", "Unknown error") << "\n";
    }
    return false;
}

std::vector<std::vector<std::string>> API::Booking::modify(std::string id,
                                                           std::string field,
                                                           std::string value) {
    API::ApiClient& client = API::ApiClient::getInstance();
    cpr::Response apiReturn =
        client.patch("/admin/booking/update/" + id,
                     nlohmann::json{{"field", field}, {"value", value}});
    if (apiReturn.status_code != 200) {
        throw std::runtime_error("Request failed with status: " +
                                 std::to_string(apiReturn.status_code));
    }

    std::vector<std::vector<std::string>> data = {
        {"ID", "Flight ID", "Account ID", "Payment Option", "Payment Detail",
         "Status", "Departure Date", "Created At", "Updated At"}};
    nlohmann::json newData;

    try {
        newData = nlohmann::json::parse(apiReturn.text);
    } catch (const nlohmann::json::parse_error& e) {
        throw std::runtime_error(std::string("Failed to parse response: ") +
                                 e.what());
    }

    data.push_back({std::to_string(newData["id"].get<long long>()),
                    newData["flight_id"].get<std::string>(),
                    std::to_string(newData["account_id"].get<long long>()),
                    newData["payment_option"].get<std::string>(),
                    newData["payment_detail"].dump(),
                    newData["booking_status"].get<std::string>(),
                    newData["departure_date"].get<std::string>(),
                    newData["created_at"].get<std::string>(),
                    newData["updated_at"].get<std::string>()});
    return data;
}

bool API::Booking::remove(std::string id) {
    API::ApiClient& client = API::ApiClient::getInstance();
    cpr::Response apiReturn = client.del("/admin/booking/delete/" + id);
    if (apiReturn.status_code == 200) {
        return true;
    } else {
        return false;
    }
}