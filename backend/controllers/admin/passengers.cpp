#include "../../utils/utils.h"
#include "../api_admin.h"

void api::admin::view_passengers(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback) {
    orm::DbClientPtr dbClient = drogon::app().getDbClient("main");
    dbClient->execSqlAsync(
        "SELECT id, frequent_flyer_code, title, first_name, last_name, "
        "birthdate, contact_email, emergency_contact_name, associated_to, "
        "created_at, updated_at, middle_name, gender, phone_number, "
        "emergency_contact_phone FROM passenger;",
        [callback](const drogon::orm::Result& result) {
            Json::Value jsonResponse;
            for (const orm::Row& row : result) {
                Json::Value rowResult;
                rowResult["id"] = (Json::Int64)row["id"].as<long long>();
                rowResult["frequent_flyer_code"] =
                    row["frequent_flyer_code"].as<std::string>();
                rowResult["title"] = row["title"].as<std::string>();
                rowResult["first_name"] = row["first_name"].as<std::string>();
                rowResult["last_name"] = row["last_name"].as<std::string>();
                rowResult["birthdate"] = row["birthdate"].as<std::string>();
                rowResult["contact_email"] =
                    row["contact_email"].as<std::string>();
                rowResult["emergency_contact_name"] =
                    row["emergency_contact_name"].as<std::string>();
                rowResult["associated_to"] =
                    (Json::Int64)row["associated_to"].as<long long>();
                rowResult["created_at"] = row["created_at"].as<std::string>();
                rowResult["updated_at"] = row["updated_at"].as<std::string>();
                rowResult["middle_name"] = row["middle_name"].as<std::string>();
                rowResult["gender"] = row["gender"].as<std::string>();
                rowResult["phone_number"] =
                    row["phone_number"].as<std::string>();
                rowResult["emergency_contact_phone"] =
                    row["emergency_contact_phone"].as<std::string>();
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

void api::admin::view_single_passenger(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback, std::string id) {
    orm::DbClientPtr dbClient = drogon::app().getDbClient("main");
    dbClient->execSqlAsync(
        "SELECT id, frequent_flyer_code, title, first_name, last_name, "
        "birthdate, contact_email, emergency_contact_name, associated_to, "
        "created_at, updated_at, middle_name, gender, phone_number, "
        "emergency_contact_phone FROM passenger WHERE id = $1;",
        [callback](const drogon::orm::Result& result) {
            if (result.empty()) {
                callback(Skybridge::Utils::error(
                    "Passenger not found", k404NotFound,
                    Json::Value("No passenger with that ID exists")));
                return;
            }
            Json::Value jsonResponse;
            auto row = result[0];
            jsonResponse["id"] = (Json::Int64)row["id"].as<long long>();
            jsonResponse["frequent_flyer_code"] =
                row["frequent_flyer_code"].as<std::string>();
            jsonResponse["title"] = row["title"].as<std::string>();
            jsonResponse["first_name"] = row["first_name"].as<std::string>();
            jsonResponse["last_name"] = row["last_name"].as<std::string>();
            jsonResponse["birthdate"] = row["birthdate"].as<std::string>();
            jsonResponse["contact_email"] =
                row["contact_email"].as<std::string>();
            jsonResponse["emergency_contact_name"] =
                row["emergency_contact_name"].as<std::string>();
            jsonResponse["associated_to"] =
                (Json::Int64)row["associated_to"].as<long long>();
            jsonResponse["created_at"] = row["created_at"].as<std::string>();
            jsonResponse["updated_at"] = row["updated_at"].as<std::string>();
            jsonResponse["middle_name"] = row["middle_name"].as<std::string>();
            jsonResponse["gender"] = row["gender"].as<std::string>();
            jsonResponse["phone_number"] =
                row["phone_number"].as<std::string>();
            jsonResponse["emergency_contact_phone"] =
                row["emergency_contact_phone"].as<std::string>();
            callback(HttpResponse::newHttpJsonResponse(jsonResponse));
        },
        [callback](const drogon::orm::DrogonDbException& e) {
            callback(Skybridge::Utils::error("Database error",
                                             k500InternalServerError,
                                             Json::Value(e.base().what())));
        },
        id);
}

// void api::admin::add_passenger(
//     const HttpRequestPtr& req,
//     std::function<void(const HttpResponsePtr&)>&& callback) {
//     std::shared_ptr<Json::Value> json = req->getJsonObject();
//     if (!json) {
//         callback(Skybridge::Utils::error("Invalid JSON", k400BadRequest));
//         return;
//     }
//     std::vector<std::string> errors = Skybridge::Utils::validateRequest(
//         *json, api::admin::add_passenger_schema());
//     if (!errors.empty()) {
//         Json::Value body;
//         for (std::basic_string<char>& e : errors) body["details"].append(e);
//         callback(Skybridge::Utils::error("Validation failed", k400BadRequest,
//                                          body["details"]));
//         return;
//     }
//     orm::DbClientPtr dbClient = drogon::app().getDbClient("main");
//     dbClient->execSqlAsync(
//         "INSERT INTO passenger (frequent_flyer_code, title, first_name, "
//         "last_name, "
//         "birthdate, contact_email, emergency_contact_name, associated_to, "
//         "middle_name, gender, phone_number, emergency_contact_phone) "
//         "VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10, $11, $12) "
//         "RETURNING id, frequent_flyer_code, title, first_name, last_name, "
//         "birthdate, contact_email, emergency_contact_name, associated_to, "
//         "created_at, updated_at, middle_name, gender, phone_number, "
//         "emergency_contact_phone;",
//         [callback](const drogon::orm::Result& result) {
//             Json::Value jsonResponse;
//             auto row = result[0];
//             jsonResponse["id"] = (Json::Int64)row["id"].as<long long>();
//             jsonResponse["frequent_flyer_code"] =
//                 row["frequent_flyer_code"].as<std::string>();
//             jsonResponse["title"] = row["title"].as<std::string>();
//             jsonResponse["first_name"] = row["first_name"].as<std::string>();
//             jsonResponse["last_name"] = row["last_name"].as<std::string>();
//             jsonResponse["birthdate"] = row["birthdate"].as<std::string>();
//             jsonResponse["contact_email"] =
//                 row["contact_email"].as<std::string>();
//             jsonResponse["emergency_contact_name"] =
//                 row["emergency_contact_name"].as<std::string>();
//             jsonResponse["associated_to"] =
//                 (Json::Int64)row["associated_to"].as<long long>();
//             jsonResponse["created_at"] = row["created_at"].as<std::string>();
//             jsonResponse["updated_at"] = row["updated_at"].as<std::string>();
//             jsonResponse["middle_name"] =
//             row["middle_name"].as<std::string>(); jsonResponse["gender"] =
//             row["gender"].as<std::string>(); jsonResponse["phone_number"] =
//                 row["phone_number"].as<std::string>();
//             jsonResponse["emergency_contact_phone"] =
//                 row["emergency_contact_phone"].as<std::string>();
//             callback(HttpResponse::newHttpJsonResponse(jsonResponse));
//         },
//         [callback](const drogon::orm::DrogonDbException& e) {
//             callback(Skybridge::Utils::error("Database error",
//                                              k500InternalServerError,
//                                              Json::Value(e.base().what())));
//         },
//         (*json)["frequent_flyer_code"].asString(),
//         (*json)["title"].asString(),
//         (*json)["first_name"].asString(), (*json)["last_name"].asString(),
//         (*json)["birthdate"].asString(), (*json)["contact_email"].asString(),
//         (*json)["emergency_contact_name"].asString(),
//         (*json)["associated_to"].asInt64(),
//         (*json)["middle_name"].asString(),
//         (*json)["gender"].asString(), (*json)["phone_number"].asString(),
//         (*json)["emergency_contact_phone"].asString());
// }

static const std::regex passengerIdPattern("^[0-9]+$");
static const std::set<std::string> ALLOWED_PASSENGER = {
    "frequent_flyer_code",
    "title",
    "first_name",
    "last_name",
    "birthdate",
    "contact_email",
    "emergency_contact_name",
    "associated_to",
    "middle_name",
    "gender",
    "phone_number",
    "emergency_contact_phone"};

void api::admin::update_passenger(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback, std::string id) {
    if (!Skybridge::Utils::is_valid_input(id, passengerIdPattern)) {
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
        if (!ALLOWED_PASSENGER.count(item["field"].asString())) {
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
            "UPDATE passenger SET " + field +
            " = $1 WHERE id = $2 RETURNING id, frequent_flyer_code, "
            "title, first_name, last_name, birthdate, contact_email, "
            "emergency_contact_name, associated_to, created_at, "
            "updated_at, middle_name, gender, phone_number, "
            "emergency_contact_phone";

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
                    row["id"] = (Json::Int64)result[0]["id"].as<long long>();
                    row["frequent_flyer_code"] =
                        result[0]["frequent_flyer_code"].as<std::string>();
                    row["title"] = result[0]["title"].as<std::string>();
                    row["first_name"] =
                        result[0]["first_name"].as<std::string>();
                    row["last_name"] = result[0]["last_name"].as<std::string>();
                    row["birthdate"] = result[0]["birthdate"].as<std::string>();
                    row["contact_email"] =
                        result[0]["contact_email"].as<std::string>();
                    row["emergency_contact_name"] =
                        result[0]["emergency_contact_name"].as<std::string>();
                    row["associated_to"] =
                        (Json::Int64)result[0]["associated_to"].as<long long>();
                    row["created_at"] =
                        result[0]["created_at"].as<std::string>();
                    row["updated_at"] =
                        result[0]["updated_at"].as<std::string>();
                    row["middle_name"] =
                        result[0]["middle_name"].as<std::string>();
                    row["gender"] = result[0]["gender"].as<std::string>();
                    row["phone_number"] =
                        result[0]["phone_number"].as<std::string>();
                    row["emergency_contact_phone"] =
                        result[0]["emergency_contact_phone"].as<std::string>();
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

void api::admin::delete_passenger(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback, std::string id) {
    orm::DbClientPtr dbClient = drogon::app().getDbClient("main");
    dbClient->execSqlAsync(
        "DELETE FROM passenger WHERE id = $1;",
        [callback](const drogon::orm::Result& result) {
            if (result.affectedRows() == 0) {
                callback(Skybridge::Utils::error("Passenger not found",
                                                 k404NotFound, Json::Value()));
                return;
            }
            Json::Value jsonResponse;
            jsonResponse["message"] = "Passenger deleted successfully";
            callback(HttpResponse::newHttpJsonResponse(jsonResponse));
        },
        [callback](const drogon::orm::DrogonDbException& e) {
            callback(Skybridge::Utils::error("Database error",
                                             k500InternalServerError,
                                             Json::Value(e.base().what())));
        },
        id);
}