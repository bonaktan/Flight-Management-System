#include "../../utils/utils.h"
#include "../api_admin.h"

void api::admin::add_booking(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback) {
    std::shared_ptr<Json::Value> json = req->getJsonObject();
    if (!json) {
        callback(Skybridge::Utils::error("Invalid JSON", k400BadRequest));
        return;
    }

    std::vector<std::string> errors = Skybridge::Utils::validateRequest(
        *json, api::admin::add_admin_booking_schema());
    if (!errors.empty()) {
        Json::Value body;
        for (std::basic_string<char>& e : errors) body["details"].append(e);
        callback(Skybridge::Utils::error("Validation failed", k400BadRequest,
                                         body["details"]));
        return;
    }

    orm::DbClientPtr dbClient = drogon::app().getDbClient("main");
    dbClient->execSqlAsync(
        "INSERT INTO booking (flight_id, account_id, payment_option, "
        "payment_detail, booking_status, departure_date) "
        "VALUES ($1, $2, $3, $4, $5, $6) "
        "RETURNING id, flight_id, account_id, payment_option, payment_detail, "
        "booking_status, created_at, updated_at, departure_date;",
        [callback](const drogon::orm::Result& result) {
            Json::Value jsonResponse;
            auto row = result[0];
            jsonResponse["id"] = (Json::Int64)row["id"].as<long long>();
            jsonResponse["flight_id"] = row["flight_id"].as<std::string>();
            jsonResponse["account_id"] = (Json::Int64)row["account_id"].as<long long>();
            jsonResponse["payment_option"] = row["payment_option"].as<std::string>();
            jsonResponse["payment_detail"] = Skybridge::Utils::parseJsonField(
                row["payment_detail"].as<std::string>());
            jsonResponse["booking_status"] = row["booking_status"].as<std::string>();
            jsonResponse["created_at"] = row["created_at"].as<std::string>();
            jsonResponse["updated_at"] = row["updated_at"].as<std::string>();
            jsonResponse["departure_date"] = row["departure_date"].as<std::string>();
            callback(HttpResponse::newHttpJsonResponse(jsonResponse));
        },
        [callback](const drogon::orm::DrogonDbException& e) {
            callback(Skybridge::Utils::error("Database error",
                                             k500InternalServerError,
                                             Json::Value(e.base().what())));
        },
        (*json)["flight_id"].asString(),
        (*json)["account_id"].asInt64(),
        (*json)["payment_option"].asString(),
        (*json)["payment_detail"].toStyledString(),
        (*json)["booking_status"].asString(),
        (*json)["departure_date"].asString());
}

void api::admin::view_bookings(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback) {
    orm::DbClientPtr dbClient = drogon::app().getDbClient("main");
    dbClient->execSqlAsync(
        "SELECT id, flight_id, account_id, payment_option, payment_detail, "
        "booking_status, created_at, updated_at, departure_date FROM booking;",
        [callback](const drogon::orm::Result& result) {
            Json::Value jsonResponse;
            for (const orm::Row& row : result) {
                Json::Value rowResult;
                rowResult["id"] = (Json::Int64)row["id"].as<long long>();
                rowResult["flight_id"] = row["flight_id"].as<std::string>();
                rowResult["account_id"] = (Json::Int64)row["account_id"].as<long long>();
                rowResult["payment_option"] = row["payment_option"].as<std::string>();
                rowResult["payment_detail"] = Skybridge::Utils::parseJsonField(
                    row["payment_detail"].as<std::string>());
                rowResult["booking_status"] = row["booking_status"].as<std::string>();
                rowResult["created_at"] = row["created_at"].as<std::string>();
                rowResult["updated_at"] = row["updated_at"].as<std::string>();
                rowResult["departure_date"] = row["departure_date"].as<std::string>();
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

void api::admin::view_single_booking(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback, std::string id) {
    orm::DbClientPtr dbClient = drogon::app().getDbClient("main");
    dbClient->execSqlAsync(
        "SELECT id, flight_id, account_id, payment_option, payment_detail, "
        "booking_status, created_at, updated_at, departure_date FROM booking WHERE id = $1;",
        [callback](const drogon::orm::Result& result) {
            if (result.empty()) {
                callback(Skybridge::Utils::error(
                    "Booking not found", k404NotFound,
                    Json::Value("No Booking with that ID exists")));
                return;
            }
            Json::Value jsonResponse;
            auto row = result[0];
            jsonResponse["id"] = (Json::Int64)row["id"].as<long long>();
            jsonResponse["flight_id"] = row["flight_id"].as<std::string>();
            jsonResponse["account_id"] = (Json::Int64)row["account_id"].as<long long>();
            jsonResponse["payment_option"] = row["payment_option"].as<std::string>();
            jsonResponse["payment_detail"] = Skybridge::Utils::parseJsonField(
                row["payment_detail"].as<std::string>());
            jsonResponse["booking_status"] = row["booking_status"].as<std::string>();
            jsonResponse["created_at"] = row["created_at"].as<std::string>();
            jsonResponse["updated_at"] = row["updated_at"].as<std::string>();
            jsonResponse["departure_date"] = row["departure_date"].as<std::string>();
            callback(HttpResponse::newHttpJsonResponse(jsonResponse));
        },
        [callback](const drogon::orm::DrogonDbException& e) {
            callback(Skybridge::Utils::error("Database error",
                                             k500InternalServerError,
                                             Json::Value(e.base().what())));
        },
        id);
}

static const std::regex bookingIdPattern("^[0-9]+$");
void api::admin::update_booking(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback, std::string id) {
    if (!Skybridge::Utils::is_valid_input(id, bookingIdPattern)) {
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
    const std::set<std::string> ALLOWED = {
        "flight_id", "account_id", "payment_option",
        "payment_detail", "booking_status", "departure_date"};
    std::string field = (*json)["field"].asString();
    if (!ALLOWED.count(field)) {
        callback(Skybridge::Utils::error("Invalid field", k400BadRequest, {}));
        return;
    }
    std::string sql = "UPDATE booking SET " + field +
                      " = $1 WHERE id = $2 RETURNING id, flight_id, account_id, "
                      "payment_option, payment_detail, booking_status, "
                      "created_at, updated_at, departure_date";
    orm::DbClientPtr dbClient = drogon::app().getDbClient("main");
    dbClient->execSqlAsync(
        sql,
        [callback](const drogon::orm::Result& result) {
            Json::Value jsonResponse;
            jsonResponse["id"] = (Json::Int64)result[0]["id"].as<long long>();
            jsonResponse["flight_id"] = result[0]["flight_id"].as<std::string>();
            jsonResponse["account_id"] = (Json::Int64)result[0]["account_id"].as<long long>();
            jsonResponse["payment_option"] = result[0]["payment_option"].as<std::string>();
            jsonResponse["payment_detail"] = Skybridge::Utils::parseJsonField(
                result[0]["payment_detail"].as<std::string>());
            jsonResponse["booking_status"] = result[0]["booking_status"].as<std::string>();
            jsonResponse["created_at"] = result[0]["created_at"].as<std::string>();
            jsonResponse["updated_at"] = result[0]["updated_at"].as<std::string>();
            jsonResponse["departure_date"] = result[0]["departure_date"].as<std::string>();
            callback(HttpResponse::newHttpJsonResponse(jsonResponse));
        },
        [callback](const drogon::orm::DrogonDbException& e) {
            callback(Skybridge::Utils::error("Database error",
                                             k500InternalServerError,
                                             Json::Value(e.base().what())));
        },
        (*json)["value"].asString(), id);
}

void api::admin::delete_booking(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback, std::string id) {
    orm::DbClientPtr dbClient = drogon::app().getDbClient("main");
    dbClient->execSqlAsync(
        "DELETE FROM booking WHERE id = $1;",
        [callback](const drogon::orm::Result& result) {
            if (result.affectedRows() == 0) {
                callback(Skybridge::Utils::error("Booking not found",
                                                 k404NotFound, Json::Value()));
                return;
            }
            Json::Value jsonResponse;
            jsonResponse["message"] = "Booking deleted successfully";
            callback(HttpResponse::newHttpJsonResponse(jsonResponse));
        },
        [callback](const drogon::orm::DrogonDbException& e) {
            callback(Skybridge::Utils::error("Database error",
                                             k500InternalServerError,
                                             Json::Value(e.base().what())));
        },
        id);
}