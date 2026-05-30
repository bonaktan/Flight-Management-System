#include "../api_admin.h"
#include "../../utils/utils.h"

void api::admin::dashboard(const HttpRequestPtr& req,
                      std::function<void(const HttpResponsePtr&)>&& callback) {
    orm::DbClientPtr dbClient = drogon::app().getDbClient("main");
    dbClient->execSqlAsync(
        "WITH flight_data AS (SELECT departure, frequency, DATE('2026-06-01' AT TIME ZONE 'Asia/Manila') AS target_date, (departure AT TIME ZONE 'Asia/Manila')::date AS dep_date FROM flight), computed AS (SELECT departure, frequency, date_part('epoch', target_date::timestamp - dep_date::timestamp) AS diff_epoch, date_part('epoch', frequency) AS freq_epoch FROM flight_data WHERE dep_date <= target_date) SELECT COUNT(*) AS departure_amt FROM computed WHERE MOD(diff_epoch::bigint, freq_epoch::bigint) = 0;",
        [callback](const drogon::orm::Result& result) {
            Json::Value jsonResponse;
            jsonResponse["departure_amt"] = result[0]["departure_amt"].as<int>();
            jsonResponse["scheduled_flights_today"] = 0; // TODO
            jsonResponse["total_revenue_today"] = 0.0; // TODO
            jsonResponse["next_flight"] = ""; // TODO
            
            callback(HttpResponse::newHttpJsonResponse(jsonResponse));
        },
        [callback](const drogon::orm::DrogonDbException& e) {
            callback(Skybridge::Utils::error("Database error",
                                             k500InternalServerError,
                                             Json::Value(e.base().what())));
            });
}
