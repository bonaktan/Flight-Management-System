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
        return true;
    } else {
        return false;
    }
}
