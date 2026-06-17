#include "../../utils/utils.h"
#include "../api_admin.h"
#include "bcrypt/BCrypt.hpp"

void api::admin::view_account(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback) {
    orm::DbClientPtr dbClient = drogon::app().getDbClient("main");
    dbClient->execSqlAsync(
        "SELECT id, account_name, email, permissions, created_at, updated_at "
        "FROM account;",
        [callback](const drogon::orm::Result& result) {
            Json::Value jsonResponse;
            for (const orm::Row& row : result) {
                Json::Value rowResult;
                rowResult["id"] = (Json::Int64)row["id"].as<long long>();
                rowResult["account_name"] =
                    row["account_name"].as<std::string>();
                rowResult["email"] = row["email"].as<std::string>();
                rowResult["permissions"] = row["permissions"].as<std::string>();
                rowResult["created_at"] = row["created_at"].as<std::string>();
                rowResult["updated_at"] = row["updated_at"].as<std::string>();
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

void api::admin::view_single_account(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback, std::string id) {
    orm::DbClientPtr dbClient = drogon::app().getDbClient("main");
    dbClient->execSqlAsync(
        "SELECT id, account_name, email, permissions "
        "FROM account WHERE id = $1;",
        [callback](const drogon::orm::Result& result) {
            if (result.empty()) {
                callback(Skybridge::Utils::error(
                    "Account not found", k404NotFound,
                    Json::Value("No account with that ID exists")));
                return;
            }
            Json::Value jsonResponse;
            auto row = result[0];
            jsonResponse["id"] = (Json::Int64)row["id"].as<long long>();
            jsonResponse["account_name"] =
                row["account_name"].as<std::string>();
            jsonResponse["email"] = row["email"].as<std::string>();
            Json::Value jsonPerms = Skybridge::Utils::parseJsonField(
                row["permissions"].as<std::string>());
            jsonResponse["permissions"] = jsonPerms;
            callback(HttpResponse::newHttpJsonResponse(jsonResponse));
        },
        [callback](const drogon::orm::DrogonDbException& e) {
            callback(Skybridge::Utils::error("Database error",
                                             k500InternalServerError,
                                             Json::Value(e.base().what())));
        },
        id);
}
static const std::regex idPattern("^[0-9]+$");
static const std::set<std::string> ALLOWED = {"account_name", "email",
                                              "permissions", "password"};
void api::admin::update_account(
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

    // Validate all fields before touching the DB
    for (const auto& item : *json) {
        if (!ALLOWED.count(item["field"].asString())) {
            callback(Skybridge::Utils::error(
                "Invalid field: " + item["field"].asString(), k400BadRequest));
            return;
        }
    }

    orm::DbClientPtr dbClient = drogon::app().getDbClient("main");

    // Shared state across async callbacks
    auto results = std::make_shared<Json::Value>(Json::arrayValue);
    auto total = std::make_shared<int>((int)json->size());
    auto completed = std::make_shared<std::atomic<int>>(0);
    auto hadError = std::make_shared<std::atomic<bool>>(false);

    for (const auto& item : *json) {
        std::string field = item["field"].asString();
        std::string value;
        if (field == "password") {
            value = BCrypt::generateHash(item["value"].asString(), 12);
            field = "password_hash";
        } else
            value = item["value"].asString();

        std::string sql =
            (field == "permissions")
                ? "UPDATE account SET permissions = $1::jsonb WHERE id = "
                  "$2 "
                  "RETURNING id, account_name, email, permissions"
                : "UPDATE account SET " + field +
                      " = $1 WHERE id = $2 RETURNING id, account_name, "
                      "email, "  // $2a$12$p0IpL8Z0LRELto2OuASBW.hjUazoSDpWqUOHLiyCcQEjXLRtAfkaq
                      "permissions";

        dbClient->execSqlAsync(
            sql,
            [callback, results, total, completed, hadError,
             field](const drogon::orm::Result& result) {
                if (result.empty()) {
                    // row not found — still count it
                    Json::Value row;
                    row["field"] = field;
                    row["status"] = "not_found";
                    results->append(row);
                } else {
                    Json::Value row;
                    row["field"] = field;
                    row["status"] = "updated";
                    row["id"] = (Json::Int64)result[0]["id"].as<long long>();
                    row["account_name"] =
                        result[0]["account_name"].as<std::string>();
                    row["email"] = result[0]["email"].as<std::string>();
                    row["permissions"] = Skybridge::Utils::parseJsonField(
                        result[0]["permissions"].as<std::string>());
                    results->append(row);
                }

                if (++(*completed) == *total && !hadError->load()) {
                    callback(HttpResponse::newHttpJsonResponse(*results));
                }
            },
            [callback, results, total, completed, hadError,
             field](const drogon::orm::DrogonDbException& e) {
                // Only send the error response once
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

void api::admin::delete_account(
    const HttpRequestPtr& req,
    std::function<void(const HttpResponsePtr&)>&& callback, std::string id) {
    orm::DbClientPtr dbClient = drogon::app().getDbClient("main");
    dbClient->execSqlAsync(
        "DELETE FROM account WHERE id = $1;",
        [callback](const drogon::orm::Result& result) {
            if (result.affectedRows() == 0) {
                callback(Skybridge::Utils::error("Account not found",
                                                 k404NotFound, Json::Value()));
                return;
            }
            Json::Value jsonResponse;
            jsonResponse["message"] = "Account deleted successfully";
            callback(HttpResponse::newHttpJsonResponse(jsonResponse));
        },
        [callback](const drogon::orm::DrogonDbException& e) {
            callback(Skybridge::Utils::error("Database error",
                                             k500InternalServerError,
                                             Json::Value(e.base().what())));
        },
        id);
};