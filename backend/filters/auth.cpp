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