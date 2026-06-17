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
    cpr::Response response = client.get("/admin/account/view/" + id);

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
        {"id", "account_name", "email", "permissions"}};
    data.push_back({std::to_string(apiReturn["id"].get<int>()),
                    apiReturn["account_name"].get<std::string>(),
                    apiReturn["email"].get<std::string>(),
                    apiReturn["permissions"].dump()});
    return data;
}

bool API::Account::add(const std::map<std::string, std::string>& fields) {
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
    return false;
}

bool API::Account::modify(
    std::string id, std::map<std::string, std::string> fields) {
    API::ApiClient& client = API::ApiClient::getInstance();

    nlohmann::json payload = nlohmann::json::array();
    for (const auto& [field, value] : fields) {
        payload.push_back({{"field", field}, {"value", value}});
    }

    cpr::Response apiReturn =
        client.patch("/admin/account/update/" + id, payload);

    if (apiReturn.status_code != 200) {
        return false;
    }
    return true;
}

bool API::Account::remove(std::string id) {
    API::ApiClient& client = API::ApiClient::getInstance();
    cpr::Response apiReturn = client.del("/admin/account/delete/" + id);
    return true;
}