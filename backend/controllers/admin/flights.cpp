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
        "base_ticket_price, flight_time, departure, frequency, airplane_id) VALUES ($1, $2, "
        "$3, $4, $5, $6, $7, $8);",
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
        (*json)["frequency"].asString(),
        (*json)["airplane"].asString());
}

static const std::regex idPattern("^SKY[0-9]{3}$");
void api::admin::update_flight(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback, std::string id) {
    if (!Skybridge::Utils::is_valid_input(id, idPattern)) {
        callback(Skybridge::Utils::error("Invalid ID", k400BadRequest));
        return;
    }
    std::shared_ptr<Json::Value> json = req->getJsonObject();
    if (!json) {
        callback(Skybridge::Utils::error("Invalid JSON", k400BadRequest));
        return;
    }
    std::vector<std::string> errors =
        Skybridge::Utils::validateRequest(*json, api::admin::update_schema());
    if (!errors.empty()) {
        Json::Value body;
        for (std::basic_string<char>& e : errors) body["details"].append(e);
        callback(Skybridge::Utils::error("Validation failed", k400BadRequest,
                                         body["details"]));
        return;
    }
    const std::set<std::string> ALLOWED = {"departure_airport_id", "arrival_airport_id",
                                           "base_ticket_price", "flight_time",
                                           "departure", "frequency", "airplane_id"};
    std::string field = (*json)["field"].asString();
    if (!ALLOWED.count(field)) {
        callback(Skybridge::Utils::error("Invalid field", k400BadRequest, {}));
        return;
    }
    std::string sql = "UPDATE flight SET " + field +
                      " = $1 WHERE id = $2 RETURNING id, departure_airport_id, "
                      "arrival_airport_id, base_ticket_price, flight_time, "
                      "departure, frequency, created_at, airplane_id";
    orm::DbClientPtr dbClient = drogon::app().getDbClient("main");
    dbClient->execSqlAsync(
        sql,
        [callback](const drogon::orm::Result& result) {
            Json::Value jsonResponse;
            jsonResponse["id"] = result[0]["id"].as<std::string>();
            jsonResponse["departure_airport_id"] = result[0]["departure_airport_id"].as<std::string>();
            jsonResponse["arrival_airport_id"] = result[0]["arrival_airport_id"].as<std::string>();
            jsonResponse["base_ticket_price"] = result[0]["base_ticket_price"].as<double>();
            jsonResponse["flight_time"] = result[0]["flight_time"].as<std::string>();
            jsonResponse["departure"] = result[0]["departure"].as<std::string>();
            jsonResponse["frequency"] = result[0]["frequency"].as<std::string>();
            jsonResponse["created_at"] = result[0]["created_at"].as<std::string>();
            jsonResponse["airplane_id"] = result[0]["airplane_id"].as<std::string>();
            callback(HttpResponse::newHttpJsonResponse(jsonResponse));
        },
        [callback](const drogon::orm::DrogonDbException& e) {
            callback(Skybridge::Utils::error("Database error",
                                             k500InternalServerError,
                                             Json::Value(e.base().what())));
        },
        (*json)["value"].asString(), id);
}

void api::admin::delete_flight(const HttpRequestPtr& req,
              std::function<void(const HttpResponsePtr&)>&& callback,
              std::string id) {
    orm::DbClientPtr dbClient = drogon::app().getDbClient("main");
    dbClient->execSqlAsync(
        "DELETE FROM flight WHERE id = $1;",
        [callback](const drogon::orm::Result& result) {
            if (result.affectedRows() == 0) {
                callback(Skybridge::Utils::error("Flight not found",
                                                 k404NotFound,
                                                 Json::Value()));
                return;
            }
            Json::Value jsonResponse;
            jsonResponse["message"] = "Flight deleted successfully";
            callback(HttpResponse::newHttpJsonResponse(jsonResponse));
        },
        [callback](const drogon::orm::DrogonDbException& e) {
            callback(Skybridge::Utils::error("Database error",
                                             k500InternalServerError,
                                             Json::Value(e.base().what())));
        }, id);
}