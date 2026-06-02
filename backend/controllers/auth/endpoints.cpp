#include <string>

#include "../../utils/utils.h"
#include "../api_auth.h"
#include "bcrypt/BCrypt.hpp"
using namespace api;

void auth::signup(const HttpRequestPtr& req,
                  std::function<void(const HttpResponsePtr&)>&& callback) {
    std::shared_ptr<Json::Value> json = req->getJsonObject();
    if (!json) {
        callback(Skybridge::Utils::error("Invalid JSON", k400BadRequest));
        return;
    }
    std::vector<std::string> errors =
        Skybridge::Utils::validateRequest(*json, api::auth::signup_schema());
    if (!errors.empty()) {
        Json::Value body;
        for (std::basic_string<char>& e : errors) body["details"].append(e);

        callback(Skybridge::Utils::error("Validation failed", k400BadRequest,
                                         body["details"]));
        return;
    }

    std::string hash = BCrypt::generateHash((*json)["password"].asString(), 12);
    std::string name = (*json)["name"].asString();
    std::string email = (*json)["email"].asString();
    orm::DbClientPtr dbClient = drogon::app().getDbClient("main");
    dbClient->execSqlAsync(
        "INSERT INTO account (account_name, email, password_hash) VALUES ($1, "
        "$2, $3) RETURNING id;",
        [name, email, callback](const drogon::orm::Result& result) {
            Json::Value jsonResponse;
            long long account_id;
            jsonResponse["success"] = true;
            jsonResponse["name"] = name;
            
            for (const orm::Row& row : result) {
                account_id = row["id"].as<long long>();
            }
            std::string csrf = generateCsrfToken();
            std::string token = generateToken(account_id, email, csrf);

            drogon::HttpResponsePtr resp = drogon::HttpResponse::newHttpJsonResponse(jsonResponse);
            auth::setAuthCookies(resp, token, csrf);
            resp->setStatusCode(drogon::k200OK);
            callback(resp);
        },
        [callback](const drogon::orm::DrogonDbException& e) {
            std::string_view msg = e.base().what();
            if (msg.find("duplicate key value") != std::string_view::npos) {
                callback(Skybridge::Utils::error("Email already exists.",
                                                 drogon::k409Conflict));
            }
            callback(Skybridge::Utils::error("Database error",
                                             k500InternalServerError,
                                             Json::Value(e.base().what())));
        },
        name, email, hash);
}