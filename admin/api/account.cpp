#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>

#include "../controls/controls.h"
#include "../display/display.h"
#include "./api.h"

using namespace Skybridge;
API::Account* API::Account::instance = nullptr;
void API::Account::save() {
    std::ofstream f(Data::FILE_ACCOUNTS);
    for (auto& a : Data::accounts)
        f << a.id << "|" << Escape::escape(a.account_name) << "|"
          << Escape::escape(a.email) << "|" << Escape::escape(a.password_hash)
          << "|" << Escape::escape(a.permissions) << "|"
          << Escape::escape(a.created_at) << "|" << Escape::escape(a.updated_at)
          << "\n";
}

void API::Account::load() {
    Data::accounts.clear();
    std::ifstream f(Data::FILE_ACCOUNTS);
    std::string line;
    while (getline(f, line)) {
        if (line.empty()) continue;
        auto t = Escape::splitLine(line);
        if (t.size() < 7) continue;
        Structs::Account a;
        a.id = stoll(t[0]);
        a.account_name = t[1];
        a.email = t[2];
        a.password_hash = t[3];
        a.permissions = t[4];
        a.created_at = t[5];
        a.updated_at = t[6];
        Data::accounts.push_back(a);
    }
}

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
    API::Account::save();
    std::cout << "\n  [OK] Account added with ID " << a.id << "\n";
}

void API::Account::modify() {
    Display::printHeader("MODIFY ACCOUNT");
    long long id = Input::getLLInput("Enter Account ID to modify: ");
    for (auto& a : Data::accounts) {
        if (a.id == id) {
            std::cout << "  Leave blank to keep current value.\n\n";
            std::string v;
            v = Input::getInput("New Name [" + a.account_name + "]: ");
            if (!v.empty()) a.account_name = v;
            v = Input::getInput("New Email [" + a.email + "]: ");
            if (!v.empty()) a.email = v;
            v = Input::getInput("New Password Hash [***]: ");
            if (!v.empty()) a.password_hash = v;
            v = Input::getInput("New Permissions [" + a.permissions + "]: ");
            if (!v.empty()) a.permissions = v;
            a.updated_at = "NOW()";
            API::Account::save();
            std::cout << "\n  [OK] Account updated.\n";
            return;
        }
    }
    std::cout << "\n  [!!] Account not found.\n";
}

void API::Account::remove() {
    Display::printHeader("DELETE ACCOUNT");
    std::string id = Input::getInput("Enter Account ID to delete: ");
    API::ApiClient& client = API::ApiClient::getInstance();
    cpr::Response apiReturn = client.del("/admin/account/delete/" + id);
    std::cout << "\n  [OK] Account deleted.\n";
}