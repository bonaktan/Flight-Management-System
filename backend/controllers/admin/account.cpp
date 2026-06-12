#include "../../utils/utils.h"
#include "../api_admin.h"
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
static const std::regex idPattern("^[0-9].*$");
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
        for (std::basic_string<char>& e : errors) body["details"].append(e);

        callback(Skybridge::Utils::error("Validation failed", k400BadRequest,
                                         body["details"]));
        return;
    }
    const std::set<std::string> ALLOWED = {"account_name", "email",
                                           "permissions"};
    std::string field = (*json)["field"].asString();
    if (!ALLOWED.count(field)) {
        callback(Skybridge::Utils::error("Invalid field", k400BadRequest, {}));
        return;
    }
    std::string sql;
    if (field == "permissions") {
        sql =
            "UPDATE account SET permissions = $1::jsonb WHERE id = $2 "
            "RETURNING id, account_name, email, permissions";
    } else {
        sql = "UPDATE account SET " + field +
              " = $1 WHERE id = $2 RETURNING id, account_name, email, "
              "permissions";
    }
    orm::DbClientPtr dbClient = drogon::app().getDbClient("main");
    dbClient->execSqlAsync(
        sql,
        [callback](const drogon::orm::Result& result) {
            Json::Value jsonResponse;
            jsonResponse["id"] = (Json::Int64)result[0]["id"].as<long long>();
            jsonResponse["account_name"] =
                result[0]["account_name"].as<std::string>();
            jsonResponse["email"] = result[0]["email"].as<std::string>();
            Json::Value jsonPerms = Skybridge::Utils::parseJsonField(
                result[0]["permissions"].as<std::string>());
            jsonResponse["permissions"] = jsonPerms;
            callback(HttpResponse::newHttpJsonResponse(jsonResponse));
        },
        [callback](const drogon::orm::DrogonDbException& e) {
            callback(Skybridge::Utils::error("Database error",
                                             k500InternalServerError,
                                             Json::Value(e.base().what())));
        },
        (*json)["value"].asString(), id);
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