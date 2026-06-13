#include "../../utils/utils.h"
#include "../api_admin.h"

void api::admin::add_airport(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback) {
    std::shared_ptr<Json::Value> json = req->getJsonObject();
    if (!json) {
        callback(Skybridge::Utils::error("Invalid JSON", k400BadRequest));
        return;
    }

    std::vector<std::string> errors = Skybridge::Utils::validateRequest(
        *json, api::admin::add_airport_schema());
    if (!errors.empty()) {
        Json::Value body;
        for (std::basic_string<char>& e : errors) body["details"].append(e);

        callback(Skybridge::Utils::error("Validation failed", k400BadRequest,
                                         body["details"]));
        return;
    }
    orm::DbClientPtr dbClient = drogon::app().getDbClient("main");
    dbClient->execSqlAsync(
        "INSERT INTO airport (name, id, capacity, country, city) VALUES ($1, "
        "$2, $3, $4, $5);",
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
        (*json)["name"].asString(), (*json)["airport_id"].asString(),
        (*json)["capacity"].asInt(), (*json)["country"].asString(),
        (*json)["city"].asString());
}

void api::admin::view_airports(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback) {
    orm::DbClientPtr dbClient = drogon::app().getDbClient("main");
    dbClient->execSqlAsync(
        "SELECT id, name, capacity, country, city, created_at FROM airport;",
        [callback](const drogon::orm::Result& result) {
            if (result.empty()) {
                callback(Skybridge::Utils::error(
                    "Airport not found", k404NotFound,
                    Json::Value("No airport with that ID exists")));
                return;
            }
            Json::Value jsonResponse;
            for (const orm::Row& row : result) {
                Json::Value rowResult;
                rowResult["id"] = row["id"].as<std::string>();
                rowResult["name"] = row["name"].as<std::string>();
                rowResult["capacity"] = row["capacity"].as<int>();
                rowResult["country"] = result[0]["country"].as<std::string>();
                rowResult["city"] = result[0]["city"].as<std::string>();
                rowResult["created_at"] = row["created_at"].as<std::string>();
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

void api::admin::view_single_airport(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback, std::string id) {
    orm::DbClientPtr dbClient = drogon::app().getDbClient("main");
    dbClient->execSqlAsync(
        "SELECT id, name, capacity, country, city FROM airport WHERE id=$1;",
        [callback](const drogon::orm::Result& result) {
            Json::Value jsonResponse;
            Json::Value rowResult;
            jsonResponse["id"] = result[0]["id"].as<std::string>();
            jsonResponse["name"] = result[0]["name"].as<std::string>();
            jsonResponse["capacity"] = result[0]["capacity"].as<int>();
            jsonResponse["country"] = result[0]["country"].as<std::string>();
            jsonResponse["city"] = result[0]["city"].as<std::string>();
            callback(HttpResponse::newHttpJsonResponse(jsonResponse));
        },
        [callback](const drogon::orm::DrogonDbException& e) {
            callback(Skybridge::Utils::error("Database error",
                                             k500InternalServerError,
                                             Json::Value(e.base().what())));
        },
        id);
}

static const std::regex idPattern("^[A-Z]{3}$");
void api::admin::update_airport(
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
    const std::set<std::string> ALLOWED = {"name", "capacity", "country",
                                           "city"};
    std::string field = (*json)["field"].asString();
    if (!ALLOWED.count(field)) {
        callback(Skybridge::Utils::error("Invalid field", k400BadRequest, {}));
        return;
    }
    std::string sql = "UPDATE airport SET " + field +
                      " = $1 WHERE id = $2 RETURNING id, name, capacity, "
                      "country, city";
    orm::DbClientPtr dbClient = drogon::app().getDbClient("main");
    dbClient->execSqlAsync(
        sql,
        [callback](const drogon::orm::Result& result) {
            Json::Value jsonResponse;
            jsonResponse["id"] = result[0]["id"].as<std::string>();
            jsonResponse["name"] = result[0]["name"].as<std::string>();
            jsonResponse["capacity"] = result[0]["capacity"].as<int>();
            jsonResponse["country"] = result[0]["country"].as<std::string>();
            jsonResponse["city"] = result[0]["city"].as<std::string>();
            callback(HttpResponse::newHttpJsonResponse(jsonResponse));
        },
        [callback](const drogon::orm::DrogonDbException& e) {
            callback(Skybridge::Utils::error("Database error",
                                             k500InternalServerError,
                                             Json::Value(e.base().what())));
        },
        (*json)["value"].asString(), id);
}

void api::admin::delete_airport(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback, std::string id) {
    orm::DbClientPtr dbClient = drogon::app().getDbClient("main");
    // BUG: if the airport is referenced by a flight, this will throw a foreign
    // key violation error.
    dbClient->execSqlAsync(
        "DELETE FROM airport WHERE id = $1;",
        [callback](const drogon::orm::Result& result) {
            if (result.affectedRows() == 0) {
                callback(Skybridge::Utils::error("Airport not found",
                                                 k404NotFound, Json::Value()));
                return;
            }
            Json::Value jsonResponse;
            jsonResponse["message"] = "Airport deleted successfully";
            callback(HttpResponse::newHttpJsonResponse(jsonResponse));
        },
        [callback](const drogon::orm::DrogonDbException& e) {
            callback(Skybridge::Utils::error("Database error",
                                             k500InternalServerError,
                                             Json::Value(e.base().what())));
        },
        id);
}