#include <iostream>

#include "../controls/controls.h"
#include "../display/display.h"
#include "./api.h"

using namespace Skybridge;
API::Auth* API::Auth::instance = nullptr;

bool API::Auth::login(std::string& email, std::string& password) {
    API::ApiClient& client = API::ApiClient::getInstance();
    auto response =
        client.post("/auth/login",
                    nlohmann::json{{"email", email}, {"password", password}});
    if (response.status_code == 200) {
        API::Auth::user = nlohmann::json::parse(response.text)["name"];
        return true;
    };
    if (response.status_code == 401) return false;

    throw std::runtime_error("Login request failed with status: " +
                             std::to_string(response.status_code));
}
