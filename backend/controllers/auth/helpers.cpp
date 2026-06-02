#include <random>
#include <string>
#include "jwt-cpp/jwt.h"
#include "../api_auth.h"

using namespace api;

const std::string auth::generateCsrfToken() {
    static const char kChars[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, sizeof(kChars) - 2);
    std::string t(32, ' ');
    for (auto& c : t) c = kChars[dist(gen)];
    return t;
}

const std::string auth::generateToken(long long userId, const std::string& username,
                                 const std::string& csrf) {
    return jwt::create()
        .set_issuer("skybridge")
        .set_subject(std::to_string(userId))
        .set_payload_claim("username", jwt::claim(username))
        .set_payload_claim("csrf", jwt::claim(csrf))
        .set_issued_at(std::chrono::system_clock::now())
        .set_expires_at(std::chrono::system_clock::now() +
                        std::chrono::hours(1))
        .sign(jwt::algorithm::hs256{JWT_SECRET});
}

void auth::setAuthCookies(const drogon::HttpResponsePtr& resp,
                            const std::string& jwt,
                            const std::string& csrf) {
    std::cout << "test" << std::endl;
    drogon::Cookie authCookie("auth_token", jwt);
    authCookie.setHttpOnly(true);
    authCookie.setSecure(false); // WARNING: testing purposes
    authCookie.setSameSite(drogon::Cookie::SameSite::kLax);
    authCookie.setMaxAge(3600);
    authCookie.setPath("/");
    resp->addCookie(authCookie);

    // csrf_token is intentionally NOT HttpOnly — React reads it client-side
    drogon::Cookie csrfCookie("csrf_token", csrf);
    csrfCookie.setHttpOnly(false);
    csrfCookie.setSecure(false);
    csrfCookie.setSameSite(drogon::Cookie::SameSite::kLax);
    csrfCookie.setMaxAge(3600);
    csrfCookie.setPath("/");
    resp->addCookie(csrfCookie);
}