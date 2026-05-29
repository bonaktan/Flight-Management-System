#include <drogon/HttpController.h>

#include <valijson/schema.hpp>

namespace Skybridge {
namespace Utils {
std::vector<std::string> validateRequest(const Json::Value& body,
                                         const valijson::Schema& schema);
drogon::HttpResponsePtr error(
    const std::string& message,
    drogon::HttpStatusCode statusCode = drogon::k500InternalServerError,
    const Json::Value& details = Json::Value());
}  // namespace Utils
}  // namespace Skybridge