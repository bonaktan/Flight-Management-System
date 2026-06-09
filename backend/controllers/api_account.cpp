#include "api_account.h"

#include "../utils/utils.h"
using namespace api;

// Add definition of your processing function here
void account::details(const HttpRequestPtr& req,
                      std::function<void(const HttpResponsePtr&)>&& callback) {
    std::string userId = req->attributes()->get<std::string>("userId");
    orm::DbClientPtr dbClient = drogon::app().getDbClient("main");
    dbClient->execSqlAsync(
        "SELECT account_name, email FROM account WHERE id=$1",
        [callback, userId](const drogon::orm::Result& result) {
            Json::Value jsonResponse;
            jsonResponse["email"] = result[0]["email"].as<std::string>();
            jsonResponse["userId"] = userId;
            jsonResponse["account_name"] =
                result[0]["account_name"].as<std::string>();
            callback(HttpResponse::newHttpJsonResponse(jsonResponse));
        },
        [callback](const drogon::orm::DrogonDbException& e) {
            callback(Skybridge::Utils::error(
                "Database Error.", k500InternalServerError, e.base().what()));
        },
        userId);
}