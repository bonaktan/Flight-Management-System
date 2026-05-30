#include "../../utils/utils.h"
#include "../api_admin.h"



void api::admin::add_flight(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback) {
    std::shared_ptr<Json::Value> json = req->getJsonObject();
    if (!json) {
        callback(Skybridge::Utils::error("Invalid JSON", k400BadRequest));
        return;
    }

    std::vector<std::string> errors = Skybridge::Utils::validateRequest(
        *json, api::admin::add_flight_schema());
    if (!errors.empty()) {
        Json::Value body;
        for (std::basic_string<char>& e : errors) body["details"].append(e);

        callback(Skybridge::Utils::error("Validation failed", k400BadRequest,
                                         body["details"]));
        return;
    }
    orm::DbClientPtr dbClient = drogon::app().getDbClient("main");
    dbClient->execSqlAsync(
        "INSERT INTO flight (id, departure_airport_id, arrival_airport_id, "
        "base_ticket_price, flight_time, departure, frequency) VALUES ($1, $2, "
        "$3, $4, $5, $6, $7);",
        [callback](const drogon::orm::Result& result) {
            Json::Value jsonResponse;
            jsonResponse["success"] = true;
            callback(HttpResponse::newHttpJsonResponse(jsonResponse));
        },
        [callback](const drogon::orm::DrogonDbException& e) {
            callback(Skybridge::Utils::error("Database error",
                                             k500InternalServerError,
                                             Json::Value(e.base().what())));
        },
        (*json)["flight_id"].asString(),
        (*json)["departure_airport_id"].asString(),
        (*json)["arrival_airport_id"].asString(),
        (*json)["base_ticket_price"].asDouble(),
        (*json)["flight_time"].asString(),
        (*json)["start_of_operations"].asString(),
        (*json)["frequency"].asString());
}