#include "api_health.h"

using namespace api;

// Add definition of your processing function here
void health::healthcheck(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback) {
    LOG_DEBUG << "Health Check done";
    auto ret = std::make_shared<Json::Value>();
    (*ret)["backend"] = true;

    orm::DbClientPtr dbClient = drogon::app().getDbClient("main");
    dbClient->execSqlAsync(
        "SELECT version();",
        [ret, callback](const drogon::orm::Result& result) {
            (*ret)["database"] = true;
            callback(HttpResponse::newHttpJsonResponse(*ret));
        },
        [ret, callback](const drogon::orm::DrogonDbException& e) {
            (*ret)["database"] = false;
            callback(HttpResponse::newHttpJsonResponse(*ret));
        });
}