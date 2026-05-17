#include "api_health.h"

using namespace api;

// Add definition of your processing function here
void health::healthcheck(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback) {
    LOG_DEBUG << "Health Check done";
    Json::Value ret;
    ret["backend"] = true;

    orm::DbClientPtr dbClient = drogon::app().getDbClient("main");
    try {
        orm::Result result = dbClient->execSqlSync("SELECT version();");
        ret["database"] = true;
    } catch (drogon::orm::DrogonDbException& e) {
        ret["database"] = false;
    }

    HttpResponsePtr resp = HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}