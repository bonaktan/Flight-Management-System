#include "./api.h"

using namespace Skybridge;

API::ApiClient* API::ApiClient::instance = nullptr;

void API::ApiClient::checkSession(const cpr::Cookies& updated_cookies) {
    if (!updated_cookies.empty()) cookies = updated_cookies;
}

API::ApiClient& API::ApiClient::getInstance() {
    if (!instance) instance = new ApiClient();
    return *instance;
}

cpr::Response API::ApiClient::get(const std::string& endpoint) {
    auto response =
        cpr::Get(cpr::Url{API::ApiClient::base_url + endpoint}, cookies);
    checkSession(response.cookies);
    return response;
}

cpr::Response API::ApiClient::post(const std::string& endpoint,
                                   nlohmann::json payload) {
    auto response = cpr::Post(cpr::Url{base_url + endpoint},
                              cpr::Header{{"Content-Type", "application/json"}},
                              cpr::Body{payload.dump()}, cookies);
    checkSession(response.cookies);
    return response;
}

cpr::Response API::ApiClient::patch(const std::string& endpoint,
                                   nlohmann::json payload) {
    auto response = cpr::Patch(cpr::Url{base_url + endpoint},
                              cpr::Header{{"Content-Type", "application/json"}},
                              cpr::Body{payload.dump()}, cookies);
    checkSession(response.cookies);
    return response;
}


cpr::Response API::ApiClient::del(const std::string& endpoint) {
    auto response = cpr::Delete(cpr::Url{base_url + endpoint}, cookies);
    checkSession(response.cookies);
    return response;
}
