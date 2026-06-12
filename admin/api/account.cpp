#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "../controls/controls.h"
#include "../display/display.h"
#include "./api.h"

using namespace Skybridge;
API::Account* API::Account::instance = nullptr;

long long API::Account::nextId() {
    long long mx = 0;
    for (auto& a : Data::accounts) mx = std::max(mx, a.id);
    return mx + 1;
}

std::vector<std::vector<std::string>> API::Account::view() {
    API::ApiClient& client = API::ApiClient::getInstance();
    nlohmann::json apiReturn =
        nlohmann::json::parse(client.get("/admin/account/view").text);
    std::vector<std::vector<std::string>> data = {{"ID", "Account Name",
                                                   "Email", "Permissions",
                                                   "Created At", "Updated At"}};
    for (const auto& entry : apiReturn) {
        data.push_back({std::to_string(entry["id"].get<int>()),
                        entry["account_name"].get<std::string>(),
                        entry["email"].get<std::string>(),
                        entry["permissions"].get<std::string>(),
                        entry["created_at"].get<std::string>(),
                        entry["updated_at"].get<std::string>()});
    }
    return data;
}

std::vector<std::vector<std::string>> API::Account::view_one(std::string id) {
    API::ApiClient& client = API::ApiClient::getInstance();
    nlohmann::json apiReturn =
        nlohmann::json::parse(client.get("/admin/account/view/" + id).text);
    std::vector<std::vector<std::string>> data = {
        {"id", "account_name", "email", "permissions"}};
    data.push_back({std::to_string(apiReturn["id"].get<int>()),
                    apiReturn["account_name"].get<std::string>(),
                    apiReturn["email"].get<std::string>(),
                    apiReturn["permissions"].dump()});
    return data;
}

void API::Account::add() {
    Display::printHeader("ADD ACCOUNT");
    Structs::Account a;
    a.id = API::Account::nextId();
    a.account_name = Input::getInput("Account Name: ");
    a.email = Input::getInput("Email: ");
    a.password_hash = Input::getInput("Password Hash: ");
    a.permissions = Input::getInput("Permissions (JSON string, e.g. {}): ");
    a.created_at = a.updated_at = "NOW()";
    Data::accounts.push_back(a);
    std::cout << "\n  [OK] Account added with ID " << a.id << "\n";
}

std::vector<std::vector<std::string>> API::Account::modify(std::string id,
                                                           std::string field,
                                                           std::string value) {
    API::ApiClient& client = API::ApiClient::getInstance();
    cpr::Response apiReturn = client.patch(
        "/admin/account/update/" + id,  nlohmann::json{{"field", field}, {"value", value}});
    std::vector<std::vector<std::string>> data = {
        {"ID", "Account Name", "Email", "Permissions"}};
    nlohmann::json newData = nlohmann::json::parse(apiReturn.text);
        data.push_back({std::to_string(newData["id"].get<int>()),
                        newData["account_name"].get<std::string>(),
                        newData["email"].get<std::string>(),
                        newData["permissions"].dump()});
    return data;
}

void API::Account::remove() {
    Display::printHeader("DELETE ACCOUNT");
    std::string id = Input::getInput("Enter Account ID to delete: ");
    API::ApiClient& client = API::ApiClient::getInstance();
    cpr::Response apiReturn = client.del("/admin/account/delete/" + id);
    std::cout << "\n  [OK] Account deleted.\n";
}