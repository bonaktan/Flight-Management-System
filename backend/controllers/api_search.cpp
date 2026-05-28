#include "api_search.h"

using namespace api;

void search::flights(const HttpRequestPtr& req,
                     std::function<void(const HttpResponsePtr&)>&& callback) {
    orm::DbClientPtr dbClient = drogon::app().getDbClient("main");
    dbClient->execSqlAsync(
        "SELECT * FROM flight;",
        [callback](const drogon::orm::Result& result) {
            Json::Value jsonResponse;
            
            for (const orm::Row& row : result) {
                Json::Value rowResult;
                rowResult["id"] = row["id"].as<std::string>();
                rowResult["departure_airport_id"] = row["departure_airport_id"].as<std::string>();
                rowResult["arrival_airport_id"] = row["arrival_airport_id"].as<std::string>();
                rowResult["base_ticket_price"] = row["base_ticket_price"].as<double>();
                rowResult["flight_time"] = row["flight_time"].as<std::string>();
                rowResult["departure"] = row["departure"].as<std::string>();
                rowResult["frequency"] = row["frequency"].as<std::string>();
                jsonResponse.append(rowResult);
            }
            callback(HttpResponse::newHttpJsonResponse(jsonResponse));
        },
        [callback](const drogon::orm::DrogonDbException& e) {
            Json::Value jsonResponse;
            jsonResponse["databaseError"] = true;
            LOG_ERROR << e.base().what();
            HttpResponsePtr resp =
                HttpResponse::newHttpJsonResponse(jsonResponse);
            resp->setStatusCode(k500InternalServerError);
            callback(resp);
        });
}