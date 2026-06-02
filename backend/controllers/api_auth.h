#pragma once

#include <drogon/HttpController.h>

#include <valijson/schema.hpp>

using namespace drogon;

namespace api {
// CRITICAL: find an envfile parser to get envvars for this shit. THIS IS
// HARDCODED TEMPORARILY.
const static std::string JWT_SECRET =
    "ded74b5520e3804b76a3f482c1d85439e4724b6f8f435c1cf50961c793d97744";

class auth : public drogon::HttpController<auth> {
   public:
    METHOD_LIST_BEGIN
    METHOD_ADD(auth::signup, "/signup", Post);
    // METHOD_ADD(health::healthcheck, "/", Get);
    METHOD_LIST_END

    void signup(const HttpRequestPtr& req,
                std::function<void(const HttpResponsePtr&)>&& callback);

   private:
    const valijson::Schema& signup_schema();
    const static std::string generateCsrfToken();
    const static std::string generateToken(long long userId, const std::string& username,
                                     const std::string& csrf);
    static void setAuthCookies(const drogon::HttpResponsePtr& resp,
                            const std::string& jwt,
                            const std::string& csrf);
};
}  // namespace api
