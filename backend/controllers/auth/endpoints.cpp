#include <string>

#include "../../utils/utils.h"
#include "../api_auth.h"
#include "bcrypt/BCrypt.hpp"
#include "jwt-cpp/jwt.h"

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
            long long account_id = result[0]["id"].as<long long>();
            jsonResponse["success"] = true;
            jsonResponse["name"] = name;

            std::string csrf = generateCsrfToken();
            std::string token = generateToken(account_id, name, csrf);

            drogon::HttpResponsePtr resp =
                drogon::HttpResponse::newHttpJsonResponse(jsonResponse);
            auth::setAuthCookies(resp, token, csrf);
            resp->setStatusCode(drogon::k200OK);
            callback(resp);
        },
        [callback](const drogon::orm::DrogonDbException& e) {
            std::string_view msg = e.base().what();
            if (msg.find("duplicate key value") != std::string_view::npos) {
                callback(Skybridge::Utils::error("Email already exists.",
                                                 drogon::k409Conflict));
                return;
            }
            callback(Skybridge::Utils::error("Database error",
                                             k500InternalServerError,
                                             Json::Value(e.base().what())));
        },
        name, email, hash);
}

void auth::login(const HttpRequestPtr& req,
                 std::function<void(const HttpResponsePtr&)>&& callback) {
    std::shared_ptr<Json::Value> json = req->getJsonObject();
    if (!json) {
        callback(Skybridge::Utils::error("Invalid JSON", k400BadRequest));
        return;
    }
    std::vector<std::string> errors =
        Skybridge::Utils::validateRequest(*json, api::auth::login_schema());
    if (!errors.empty()) {
        Json::Value body;
        for (std::basic_string<char>& e : errors) body["details"].append(e);
        callback(Skybridge::Utils::error(
            "Validation Failed", drogon::k401Unauthorized, body["details"]));
        return;
    }
    std::string token = req->getCookie("auth_token");
    if (!token.empty()) {
        callback(Skybridge::Utils::error("Already logged in.",
                                         drogon::k401Unauthorized,
                                         "Auth Token found."));
        return;
    }
    orm::DbClientPtr dbClient = drogon::app().getDbClient("main");
    dbClient->execSqlAsync(
        "SELECT id, account_name, email, password_hash FROM account WHERE "
        "email = $1;",
        [json, callback](const drogon::orm::Result& result) {
            bool userFound = !result.empty();
            std::string email = (*json)["email"].asString();

            static const std::string kDummyHash =
                "$2a$12$xdeF5EDQbf2PPqXq4sbIG.C2kqNSUCemfZt9gybFAMUphoWu6gVES";
            const std::string& storedHash =
                userFound ? result[0]["password_hash"].as<std::string>()
                          : kDummyHash;
            bool passwordValid = BCrypt::validatePassword(
                (*json)["password"].asString(), storedHash);

            Json::Value jsonResponse;
            std::string csrf = generateCsrfToken();

            if (!userFound || !passwordValid) {
                callback(Skybridge::Utils::error(
                    "Invalid Credentials", drogon::k401Unauthorized,
                    Json::Value("Wrong Password supplied.")));
                return;
            }
            long long account_id = result[0]["id"].as<long long>();
            std::string name = result[0]["account_name"].as<std::string>();
            jsonResponse["success"] = true;
            jsonResponse["name"] = name;
            std::string token = generateToken(account_id, name, csrf);
            drogon::HttpResponsePtr resp =
                drogon::HttpResponse::newHttpJsonResponse(jsonResponse);
            auth::setAuthCookies(resp, token, csrf);
            resp->setStatusCode(drogon::k200OK);

            callback(resp);
        },
        [callback](const drogon::orm::DrogonDbException& e) {
            callback(Skybridge::Utils::error("Database error",
                                             k500InternalServerError,
                                             Json::Value(e.base().what())));
            return;
        },
        (*json)["email"].asString());
}

void auth::logout(const HttpRequestPtr& req,
                  std::function<void(const HttpResponsePtr&)>&& callback) {
    std::string token = req->getCookie("auth_token");
    if (token.empty()) {
        callback(Skybridge::Utils::error("Already logged out.",
                                         drogon::k401Unauthorized,
                                         "Auth Token not found."));
        return;
    }
    Json::Value jsonResponse;
    jsonResponse["success"] = true;
    drogon::HttpResponsePtr resp =
        drogon::HttpResponse::newHttpJsonResponse(jsonResponse);
    clearAuthCookies(resp);
    resp->setStatusCode(drogon::k200OK);
    callback(resp);
}

void auth::authenticate(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback) {
    /* WARNING: this process doesn't touch the database at all. while it
     * pretty much "works", there is a problem wherein invalidated users
     * (deleted accts, changed passwds) are still authenticated correctly.
     * that problem will be ignored for now but should be fixed in further
     * iterations
     */
    Json::Value body;
    body["userId"] = req->attributes()->get<std::string>("userId");
    body["username"] = req->attributes()->get<std::string>("username");

    drogon::HttpResponsePtr resp =
        drogon::HttpResponse::newHttpJsonResponse(body);
    resp->setStatusCode(drogon::k200OK);
    callback(resp);
}