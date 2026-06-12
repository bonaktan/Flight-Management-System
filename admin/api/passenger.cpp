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
API::Passenger* API::Passenger::instance = nullptr;

std::vector<std::vector<std::string>> API::Passenger::view() {
    API::ApiClient& client = API::ApiClient::getInstance();
    nlohmann::json apiReturn =
        nlohmann::json::parse(client.get("/admin/passenger/view").text);
    std::vector<std::vector<std::string>> data = {
        {"ID", "Frequent Flyer", "Title", "First Name", "Middle Name",
         "Last Name", "Birthdate", "Gender", "Email", "Phone",
         "Emergency Contact", "Emergency Phone", "Associated To", "Created At",
         "Updated At"}};
    for (const auto& entry : apiReturn) {
        data.push_back({std::to_string(entry["id"].get<long long>()),
                        entry["frequent_flyer_code"].get<std::string>(),
                        entry["title"].get<std::string>(),
                        entry["first_name"].get<std::string>(),
                        entry["middle_name"].get<std::string>(),
                        entry["last_name"].get<std::string>(),
                        entry["birthdate"].get<std::string>(),
                        entry["gender"].get<std::string>(),
                        entry["contact_email"].get<std::string>(),
                        entry["phone_number"].get<std::string>(),
                        entry["emergency_contact_name"].get<std::string>(),
                        entry["emergency_contact_phone"].get<std::string>(),
                        std::to_string(entry["associated_to"].get<long long>()),
                        entry["created_at"].get<std::string>(),
                        entry["updated_at"].get<std::string>()});
    }
    return data;
}

std::vector<std::vector<std::string>> API::Passenger::view_one(std::string id) {
    API::ApiClient& client = API::ApiClient::getInstance();
    nlohmann::json apiReturn =
        nlohmann::json::parse(client.get("/admin/passenger/view/" + id).text);
    std::vector<std::vector<std::string>> data = {
        {"id", "frequent_flyer_code", "title", "first_name", "middle_name",
         "last_name", "birthdate", "gender", "contact_email", "phone_number",
         "emergency_contact_name", "emergency_contact_phone", "associated_to",
         "created_at", "updated_at"}};
    data.push_back({std::to_string(apiReturn["id"].get<long long>()),
                    apiReturn["frequent_flyer_code"].get<std::string>(),
                    apiReturn["title"].get<std::string>(),
                    apiReturn["first_name"].get<std::string>(),
                    apiReturn["middle_name"].get<std::string>(),
                    apiReturn["last_name"].get<std::string>(),
                    apiReturn["birthdate"].get<std::string>(),
                    apiReturn["gender"].get<std::string>(),
                    apiReturn["contact_email"].get<std::string>(),
                    apiReturn["phone_number"].get<std::string>(),
                    apiReturn["emergency_contact_name"].get<std::string>(),
                    apiReturn["emergency_contact_phone"].get<std::string>(),
                    std::to_string(apiReturn["associated_to"].get<long long>()),
                    apiReturn["created_at"].get<std::string>(),
                    apiReturn["updated_at"].get<std::string>()});
    return data;
}

void API::Passenger::add() {
    Display::printHeader("ADD PASSENGER");
    nlohmann::json passenger;
    passenger["frequent_flyer_code"] = Input::getInput("Frequent Flyer Code: ");
    passenger["title"] = Input::getInput("Title (Mr/Ms/Dr...): ");
    passenger["first_name"] = Input::getInput("First Name: ");
    passenger["middle_name"] = Input::getInput("Middle Name: ");
    passenger["last_name"] = Input::getInput("Last Name: ");
    passenger["birthdate"] = Input::getInput("Birthdate (YYYY-MM-DD): ");
    passenger["gender"] = Input::getInput("Gender: ");
    passenger["contact_email"] = Input::getInput("Contact Email: ");
    passenger["phone_number"] = Input::getInput("Phone Number: ");
    passenger["emergency_contact_name"] =
        Input::getInput("Emergency Contact Name: ");
    passenger["emergency_contact_phone"] =
        Input::getInput("Emergency Contact Phone: ");
    passenger["associated_to"] =
        std::stoll(Input::getInput("Associated Account ID: "));

    API::ApiClient& client = API::ApiClient::getInstance();
    cpr::Response apiReturn = client.post("/admin/passenger/add", passenger);
    if (apiReturn.status_code == 200 || apiReturn.status_code == 201) {
        std::cout << "\n  [OK] Passenger added.\n";
    } else {
        nlohmann::json errorResponse = nlohmann::json::parse(apiReturn.text);
        std::cerr << "\n  [ERROR] Failed to add passenger: "
                  << errorResponse.value("message", "Unknown error") << "\n";
    }
}

std::vector<std::vector<std::string>> API::Passenger::modify(
    std::string id, std::string field, std::string value) {
    API::ApiClient& client = API::ApiClient::getInstance();
    cpr::Response apiReturn =
        client.patch("/admin/passenger/update/" + id,
                     nlohmann::json{{"field", field}, {"value", value}});
    std::vector<std::vector<std::string>> data = {
        {"ID", "Frequent Flyer", "Title", "First Name", "Middle Name",
         "Last Name", "Birthdate", "Gender", "Email", "Phone",
         "Emergency Contact", "Emergency Phone", "Associated To", "Created At",
         "Updated At"}};
    nlohmann::json newData = nlohmann::json::parse(apiReturn.text);
    data.push_back({std::to_string(newData["id"].get<long long>()),
                    newData["frequent_flyer_code"].get<std::string>(),
                    newData["title"].get<std::string>(),
                    newData["first_name"].get<std::string>(),
                    newData["middle_name"].get<std::string>(),
                    newData["last_name"].get<std::string>(),
                    newData["birthdate"].get<std::string>(),
                    newData["gender"].get<std::string>(),
                    newData["contact_email"].get<std::string>(),
                    newData["phone_number"].get<std::string>(),
                    newData["emergency_contact_name"].get<std::string>(),
                    newData["emergency_contact_phone"].get<std::string>(),
                    std::to_string(newData["associated_to"].get<long long>()),
                    newData["created_at"].get<std::string>(),
                    newData["updated_at"].get<std::string>()});
    return data;
}

void API::Passenger::remove() {
    Display::printHeader("DELETE PASSENGER");
    std::string id = Input::getInput("Enter Passenger ID to delete: ");
    API::ApiClient& client = API::ApiClient::getInstance();
    cpr::Response apiReturn = client.del("/admin/passenger/delete/" + id);
    if (apiReturn.status_code == 200) {
        std::cout << "\n  [OK] Passenger deleted.\n";
    } else {
        nlohmann::json errorResponse = nlohmann::json::parse(apiReturn.text);
        std::cerr << "\n  [ERROR] Failed to delete passenger: "
                  << errorResponse.value("message", "Unknown error") << "\n";
    }
}