#include "../controllers/api_auth.h"
#include "../utils/utils.h"
#include "AuthFilter.h"
#include "bcrypt/BCrypt.hpp"
#include "jwt-cpp/jwt.h"

void AuthFilter::doFilter(const drogon::HttpRequestPtr& req,
                          drogon::FilterCallback&& fcb,
                          drogon::FilterChainCallback&& fccb) {
    std::string token = req->getCookie("auth_token");
    if (token.empty()) {
        fcb(Skybridge::Utils::error("Not logged in.", drogon::k401Unauthorized,
                                    "Auth Token not found."));
        return;
    }
    try {
        auto decoded = jwt::decode(token);
        jwt::verify()
            .allow_algorithm(jwt::algorithm::hs256{api::JWT_SECRET})
            .with_issuer("skybridge")
            .verify(decoded);

        req->attributes()->insert("userId", decoded.get_subject());
        req->attributes()->insert(
            "username", decoded.get_payload_claim("username").as_string());

        fccb();
    } catch (...) {
        Json::Value jsonResponse;
        jsonResponse["error"] = "Invalid or Expired token.";
        drogon::HttpResponsePtr resp =
            drogon::HttpResponse::newHttpJsonResponse(jsonResponse);
        resp->setStatusCode(drogon::k401Unauthorized);
        fcb(resp);
    }
}

void AdminFilter::doFilter(const drogon::HttpRequestPtr& req,
                           drogon::FilterCallback&& fcb,
                           drogon::FilterChainCallback&& fccb) {
    orm::DbClientPtr dbClient = drogon::app().getDbClient("main");
    dbClient->execSqlAsync(
        "SELECT permissions FROM account WHERE id=$1",
        [fcb, fccb](const drogon::orm::Result& result) {
            if (result.empty()) {
                fcb(Skybridge::Utils::error("User not found",
                                            drogon::k401Unauthorized));
                return;
            }
            try {
                Json::Value jsonResponse = Skybridge::Utils::parseJsonField(
                    result[0]["permissions"].as<std::string>());
                for (const auto& item : jsonResponse) {
                    if (item == "ADMINISTRATOR") {
                        fccb();
                        return;
                    }
                }
                fcb(Skybridge::Utils::error("No Permissions",
                                            drogon::k401Unauthorized));
            } catch (const std::exception& e) {
                fcb(Skybridge::Utils::error("Invalid JSON Format. Contact Admin for details",
                                            drogon::k500InternalServerError,
                                            Json::Value(e.what())));
            }
        },
        [fcb](const drogon::orm::DrogonDbException& e) {
            fcb(Skybridge::Utils::error("Database error",
                                        k500InternalServerError,
                                        Json::Value(e.base().what())));
        },
        req->attributes()->get<std::string>("userId"));
}