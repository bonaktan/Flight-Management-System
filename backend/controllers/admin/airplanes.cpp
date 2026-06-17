#include "../../utils/utils.h"
#include "../api_admin.h"

void api::admin::add_airplane(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback) {
    std::shared_ptr<Json::Value> json = req->getJsonObject();
    if (!json) {
        callback(Skybridge::Utils::error("Invalid JSON", k400BadRequest));
        return;
    }
    std::vector<std::string> errors = Skybridge::Utils::validateRequest(
        *json, api::admin::add_airplane_schema());
    if (!errors.empty()) {
        Json::Value body;
        for (std::basic_string<char>& e : errors) body["details"].append(e);

        callback(Skybridge::Utils::error("Validation failed", k400BadRequest,
                                         body["details"]));
        return;
    }
    Json::StreamWriterBuilder writer;
    writer["indentation"] = "";
    std::string seatmap = Json::writeString(writer, (*json)["seatmap"]);
    orm::DbClientPtr dbClient = drogon::app().getDbClient("main");
    dbClient->execSqlAsync(
        "INSERT INTO airplane (id, model, location, seatmap) VALUES ($1, $2, "
        "$3, $4);",
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
        (*json)["airplane_id"].asString(), (*json)["model"].asString(),
        (*json)["location"].asString(), seatmap);
}

void api::admin::view_airplanes(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback) {
    orm::DbClientPtr dbClient = drogon::app().getDbClient("main");
    dbClient->execSqlAsync(
        "SELECT id, model, location, seatmap, seat_class FROM airplane;",
        [callback](const drogon::orm::Result& result) {
            Json::Value jsonResponse;
            for (const orm::Row& row : result) {
                Json::Value rowResult;
                rowResult["id"] = row["id"].as<std::string>();
                rowResult["model"] = row["model"].as<std::string>();
                rowResult["location"] = row["location"].as<std::string>();
                Json::Value seatmap = Skybridge::Utils::parseJsonField(
                    row["seatmap"].as<std::string>());
                rowResult["seatmap"] = seatmap;
                Json::Value seat_class = Skybridge::Utils::parseJsonField(
                    row["seat_class"].as<std::string>());
                rowResult["seat_class"] = seat_class;
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

void api::admin::view_single_airplane(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback, std::string id) {
    orm::DbClientPtr dbClient = drogon::app().getDbClient("main");
    dbClient->execSqlAsync(
        "SELECT id, model, location, seatmap, seat_class FROM airplane WHERE "
        "id=$1;",
        [callback](const drogon::orm::Result& result) {
            if (result.empty()) {
                callback(Skybridge::Utils::error(
                    "Airplane not found", k404NotFound,
                    Json::Value("No airplane with that ID exists")));
                return;
            }
            Json::Value jsonResponse;
            jsonResponse["id"] = result[0]["id"].as<std::string>();
            jsonResponse["model"] = result[0]["model"].as<std::string>();
            jsonResponse["location"] = result[0]["location"].as<std::string>();
            Json::Value seatmap = Skybridge::Utils::parseJsonField(
                result[0]["seatmap"].as<std::string>());
            jsonResponse["seatmap"] = seatmap;
            Json::Value seat_class = Skybridge::Utils::parseJsonField(
                result[0]["seat_class"].as<std::string>());
            jsonResponse["seat_class"] = seat_class;
            callback(HttpResponse::newHttpJsonResponse(jsonResponse));
        },
        [callback](const drogon::orm::DrogonDbException& e) {
            callback(Skybridge::Utils::error("Database error",
                                             k500InternalServerError,
                                             Json::Value(e.base().what())));
        },
        id);
}

static const std::regex idPattern("^SB-[A-Z][0-9]{4}$");
static const std::set<std::string> ALLOWED_AIRPLANE = {"model", "location",
                                                       "seatmap", "seat_class"};

void api::admin::update_airplane(
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
        for (std::string& e : errors) body["details"].append(e);
        callback(Skybridge::Utils::error("Validation failed", k400BadRequest,
                                         body["details"]));
        return;
    }

    for (const auto& item : *json) {
        if (!ALLOWED_AIRPLANE.count(item["field"].asString())) {
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

        bool isJsonb = (field == "seatmap" || field == "seat_class");
        std::string sql =
            "UPDATE airplane SET " + field +
            (isJsonb ? " = $1::jsonb" : " = $1") +
            " WHERE id = $2 RETURNING id, model, location, seatmap, seat_class";

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
                    row["model"] = result[0]["model"].as<std::string>();
                    row["location"] = result[0]["location"].as<std::string>();
                    row["seatmap"] = Skybridge::Utils::parseJsonField(
                        result[0]["seatmap"].as<std::string>());
                    row["seat_class"] = Skybridge::Utils::parseJsonField(
                        result[0]["seat_class"].as<std::string>());
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

void api::admin::delete_airplane(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback, std::string id) {
    orm::DbClientPtr dbClient = drogon::app().getDbClient("main");
    // BUG: if the airplane is referenced by a flight, this will throw a foreign
    // key violation error.
    dbClient->execSqlAsync(
        "DELETE FROM airplane WHERE id = $1;",
        [callback](const drogon::orm::Result& result) {
            if (result.affectedRows() == 0) {
                callback(Skybridge::Utils::error("Airplane not found",
                                                 k404NotFound, Json::Value()));
                return;
            }
            Json::Value jsonResponse;
            jsonResponse["message"] = "Airplane deleted successfully";
            callback(HttpResponse::newHttpJsonResponse(jsonResponse));
        },
        [callback](const drogon::orm::DrogonDbException& e) {
            callback(Skybridge::Utils::error("Database error",
                                             k500InternalServerError,
                                             Json::Value(e.base().what())));
        },
        id);
}