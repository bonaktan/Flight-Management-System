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
    resp->setStatusCode(statusCode);
    return resp;
}

bool Utils::is_valid_input(const std::string& input,
                           const std::regex& pattern) {
    return std::regex_match(input, pattern);
}

Json::Value Utils::parseJsonField(const std::string& raw) {
    Json::CharReaderBuilder reader;
    std::string errs;
    Json::Value out;
    std::istringstream s(raw);
    if (!Json::parseFromStream(reader, s, &out, &errs))
        throw std::runtime_error("JSON parse error: " + errs);
    return out;
}

std::vector<std::string> Utils::parsePgArray(const std::string& pgArray) {
    std::vector<std::string> result;
    if (pgArray.size() < 2) return result;
    std::string inner = pgArray.substr(1, pgArray.size() - 2);
    std::stringstream ss(inner);
    std::string token;
    while (std::getline(ss, token, ',')) {
        result.push_back(token);
    }
    return result;
}
