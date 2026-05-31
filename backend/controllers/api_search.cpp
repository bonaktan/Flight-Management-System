#include "api_search.h"
#include "../utils/utils.h"
using namespace api;


/* /api/search/flights
 * This endpoint will return a list of all flights in the database.
 * JSON Parameters:
 *  - origin: string
 *  - destination: string
 *  - departure_date: string (YYYY-MM-DD)
 *  - timezone: string (±HH:MM)
 *  - passengers: int
*/
// BUG: apparently this is  bug, but i should not use json body data inside a GET request. sooo refactor the code to take it url query parameters instead. also update the frontend to match this change. 
// TEMPORARY FIX: swapped to a post request temporarily instead of a get request
// TODO: do search sorting and filtering options 
// (such as sort for price, flight length, etc, filter for x class only) 
void search::flights(const HttpRequestPtr& req,
                     std::function<void(const HttpResponsePtr&)>&& callback) {
    // sanity checking boilerplate
    std::shared_ptr<Json::Value> json = req -> getJsonObject();
    if (!json) {
        callback(Skybridge::Utils::error("Invalid JSON", k400BadRequest));
        return;
    }
    std::cout << "Received search request: " << (*json).toStyledString() << std::endl;
    std::vector<std::string> errors = Skybridge::Utils::validateRequest(*json, flightSchema());
    if (!errors.empty()) {
        Json::Value body;
        for (std::basic_string<char>& e : errors)
            body["details"].append(e);
        
        callback(Skybridge::Utils::error("Validation failed", k400BadRequest, body["details"]));
        return;
    }

    orm::DbClientPtr dbClient = drogon::app().getDbClient("main");
    dbClient->execSqlAsync(
        "SELECT * FROM flight WHERE departure_airport_id = $1 AND arrival_airport_id = $2 AND DATE(departure AT TIME ZONE $4) = $3;",
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
                    row["base_ticket_price"].as<float>();
                rowResult["flight_time"] = row["flight_time"].as<std::string>();
                rowResult["departure"] = row["departure"].as<std::string>();
                rowResult["frequency"] = row["frequency"].as<std::string>();
                jsonResponse.append(rowResult);
            }
            callback(HttpResponse::newHttpJsonResponse(jsonResponse));
        },
        [callback](const drogon::orm::DrogonDbException& e) {
            
            callback(Skybridge::Utils::error("Database error", k500InternalServerError, Json::Value(e.base().what())));
        },
        (*json)["origin"].asString(),
        (*json)["destination"].asString(),
        (*json)["departure_date"].asString(),
        (*json)["timezone"].asString()
    );
}