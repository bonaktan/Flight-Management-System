#include <valijson/adapters/jsoncpp_adapter.hpp>
#include <valijson/schema.hpp>
#include <valijson/validator.hpp>

#include "utils.h"

using namespace Skybridge;

std::vector<std::string> Utils::validateRequest(
    const Json::Value& body, const valijson::Schema& schema) {
    
    valijson::Validator validator(valijson::Validator::kStrongTypes);
    valijson::ValidationResults results;
    valijson::adapters::JsonCppAdapter adapter(body);

    if (validator.validate(schema, adapter, &results)) {
        return {};
    }

    std::vector<std::string> errors;
    valijson::ValidationResults::Error error;
    while (results.popError(error)) {
        std::string context;
        for (std::basic_string<char>& c : error.context) context += c;
        errors.push_back(context + ": " + error.description);
    }
    return errors;
}

drogon::HttpResponsePtr Utils::error(const std::string& message,
                                     drogon::HttpStatusCode statusCode,
                                     const Json::Value& details) {
    LOG_ERROR << message;      
    std::cout << details << std::endl;
    Json::Value jsonResponse;
    jsonResponse["error"] = message;
    drogon::HttpResponsePtr resp =
        drogon::HttpResponse::newHttpJsonResponse(jsonResponse);
    if (details.empty()) {
        jsonResponse["details"] = details["details"];
    }
    resp->setStatusCode(statusCode);
    return resp;
}

bool Utils::is_valid_input(const std::string& input, const std::regex& pattern) {
    return std::regex_match(input, pattern);
}