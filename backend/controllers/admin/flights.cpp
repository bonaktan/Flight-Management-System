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
        "base_ticket_price, flight_time, departure, frequency, airplane_id) "
        "VALUES ($1, $2, "
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
        (*json)["frequency"].asString(), (*json)["airplane"].asString());
}

void api::admin::view_flights(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback) {
    orm::DbClientPtr dbClient = drogon::app().getDbClient("main");
    dbClient->execSqlAsync(
        "SELECT id, departure_airport_id, arrival_airport_id, "
        "base_ticket_price, "
        "flight_time, departure, frequency, created_at, airplane_id FROM "
        "flight;",
        [callback](const drogon::orm::Result& result) {
            Json::Value jsonResponse;
            for (const orm::Row& row : result) {
                Json::Value rowResult;
                rowResult["id"] = row["id"].as<std::string>();
                rowResult["departure_airport_id"] =
                    row["departure_airport_id"].as<std::string>();
                rowResult["arrival_airport_id"] =
                    row["arrival_airport_id"].as<std::string>();
                rowResult["base_ticket_price"] =
                    row["base_ticket_price"].as<double>();
                rowResult["flight_time"] = row["flight_time"].as<std::string>();
                rowResult["departure"] = row["departure"].as<std::string>();
                rowResult["frequency"] = row["frequency"].as<std::string>();
                rowResult["created_at"] = row["created_at"].as<std::string>();
                rowResult["airplane_id"] = row["airplane_id"].as<std::string>();
                jsonResponse.append(rowResult);
            }
            callback(HttpResponse::newHttpJsonResponse(jsonResponse));
        },
        [callback](const drogon::orm::DrogonDbException& e) {
            callback(Skybridge::Utils::error("Database error",
                                             k500InternalServerError,
                                             Json::Value(e.base().what())));
        });
}

void api::admin::view_single_flight(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback, std::string id) {
    orm::DbClientPtr dbClient = drogon::app().getDbClient("main");
    dbClient->execSqlAsync(
        "SELECT id, departure_airport_id, arrival_airport_id, "
        "base_ticket_price, "
        "flight_time, departure, frequency, created_at, airplane_id FROM "
        "flight WHERE id = $1;",
        [callback](const drogon::orm::Result& result) {
            if (result.empty()) {
                callback(Skybridge::Utils::error(
                    "Flight not found", k404NotFound,
                    Json::Value("No Flight with that ID exists")));
                return;
            }
            Json::Value jsonResponse;
            auto row = result[0];
            jsonResponse["id"] = row["id"].as<std::string>();
            jsonResponse["departure_airport_id"] =
                row["departure_airport_id"].as<std::string>();
            jsonResponse["arrival_airport_id"] =
                row["arrival_airport_id"].as<std::string>();
            jsonResponse["base_ticket_price"] =
                row["base_ticket_price"].as<double>();
            jsonResponse["flight_time"] = row["flight_time"].as<std::string>();
            jsonResponse["departure"] = row["departure"].as<std::string>();
            jsonResponse["frequency"] = row["frequency"].as<std::string>();
            jsonResponse["created_at"] = row["created_at"].as<std::string>();
            jsonResponse["airplane_id"] = row["airplane_id"].as<std::string>();
            callback(HttpResponse::newHttpJsonResponse(jsonResponse));
        },
        [callback](const drogon::orm::DrogonDbException& e) {
            callback(Skybridge::Utils::error("Database error",
                                             k500InternalServerError,
                                             Json::Value(e.base().what())));
        },
        id);
}

static const std::regex flightIdPattern("^SKY[0-9]{3}$");
static const std::set<std::string> ALLOWED_FLIGHT = {
    "departure_airport_id", "arrival_airport_id", "base_ticket_price",
    "flight_time",          "departure",          "frequency",
    "airplane_id"};

void api::admin::update_flight(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback, std::string id) {
    if (!Skybridge::Utils::is_valid_input(id, flightIdPattern)) {
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
        for (std::string& e : errors) body["details"].append(e);
        callback(Skybridge::Utils::error("Validation failed", k400BadRequest,
                                         body["details"]));
        return;
    }

    for (const auto& item : *json) {
        if (!ALLOWED_FLIGHT.count(item["field"].asString())) {
            callback(Skybridge::Utils::error(
                "Invalid field: " + item["field"].asString(), k400BadRequest));
            return;
        }
    }

    orm::DbClientPtr dbClient = drogon::app().getDbClient("main");

    auto results = std::make_shared<Json::Value>(Json::arrayValue);
    auto total = std::make_shared<int>((int)json->size());
    auto completed = std::make_shared<std::atomic<int>>(0);
    auto hadError = std::make_shared<std::atomic<bool>>(false);

    for (const auto& item : *json) {
        std::string field = item["field"].asString();
        std::string value = item["value"].asString();

        std::string sql =
            "UPDATE flight SET " + field +
            " = $1 WHERE id = $2 RETURNING id, departure_airport_id, "
            "arrival_airport_id, base_ticket_price, flight_time, "
            "departure, frequency, created_at, airplane_id";

        dbClient->execSqlAsync(
            sql,
            [callback, results, total, completed, hadError,
             field](const drogon::orm::Result& result) {
                if (result.empty()) {
                    Json::Value row;
                    row["field"] = field;
                    row["status"] = "not_found";
                    results->append(row);
                } else {
                    Json::Value row;
                    row["field"] = field;
                    row["status"] = "updated";
                    row["id"] = result[0]["id"].as<std::string>();
                    row["departure_airport_id"] =
                        result[0]["departure_airport_id"].as<std::string>();
                    row["arrival_airport_id"] =
                        result[0]["arrival_airport_id"].as<std::string>();
                    row["base_ticket_price"] =
                        result[0]["base_ticket_price"].as<double>();
                    row["flight_time"] =
                        result[0]["flight_time"].as<std::string>();
                    row["departure"] = result[0]["departure"].as<std::string>();
                    row["frequency"] = result[0]["frequency"].as<std::string>();
                    row["created_at"] =
                        result[0]["created_at"].as<std::string>();
                    row["airplane_id"] =
                        result[0]["airplane_id"].as<std::string>();
                    results->append(row);
                }

                if (++(*completed) == *total && !hadError->load()) {
                    callback(HttpResponse::newHttpJsonResponse(*results));
                }
            },
            [callback, results, total, completed, hadError,
             field](const drogon::orm::DrogonDbException& e) {
                if (!hadError->exchange(true)) {
                    callback(Skybridge::Utils::error(
                        "Database error on field: " + field,
                        k500InternalServerError, Json::Value(e.base().what())));
                }
                ++(*completed);
            },
            value, id);
    }
}

void api::admin::delete_flight(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback, std::string id) {
    orm::DbClientPtr dbClient = drogon::app().getDbClient("main");
    dbClient->execSqlAsync(
        "DELETE FROM flight WHERE id = $1;",
        [callback](const drogon::orm::Result& result) {
            if (result.affectedRows() == 0) {
                callback(Skybridge::Utils::error("Flight not found",
                                                 k404NotFound, Json::Value()));
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
        },
        id);
}