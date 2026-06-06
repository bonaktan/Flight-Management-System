#include "api_search.h"

#include <regex>

#include "../utils/utils.h"
using namespace api;
using namespace Skybridge;

/* /api/search/flights
 * This endpoint will return a list of all flights in the database.
 * JSON Parameters:
 *  - origin: string
 *  - destination: string
 *  - departure_date: string (YYYY-MM-DD)
 *  - timezone: string (±HH:MM)
 *  - passengers: int
 */
static const std::regex originPattern("^[A-Z]{3}$");
static const std::regex destinationPattern("^[A-Z]{3}$");
static const std::regex departureDatePattern("^\\d{4}-\\d{2}-\\d{2}$");
static const std::regex timezonePattern("^[A-Za-z_]+(?:\\/[A-Za-z_+-]+)+$");
static const std::regex passengersPattern("^[1-9]$");
// TODO: do search sorting and filtering options
// (such as sort for price, flight length, etc, filter for x class only)
void search::flights(const HttpRequestPtr& req,
                     std::function<void(const HttpResponsePtr&)>&& callback) {
    // sanity checking boilerplate
    const auto& parameters = req->getParameters();
    std::string origin = req->getParameter("origin");
    std::string destination = req->getParameter("destination");
    std::string departure_date = req->getParameter("departure_date");
    std::string timezone = req->getParameter("timezone");
    std::string passengers = req->getParameter("passengers");
    if (!Utils::is_valid_input(origin, originPattern)) {
        callback(Utils::error("Invalid origin", k400BadRequest));
        return;
    }
    if (!Utils::is_valid_input(destination, destinationPattern)) {
        callback(Utils::error("Invalid destination", k400BadRequest));
        return;
    }
    if (!Utils::is_valid_input(departure_date, departureDatePattern)) {
        callback(Utils::error("Invalid departure date", k400BadRequest));
        return;
    }
    if (!Utils::is_valid_input(timezone, timezonePattern)) {
        callback(Utils::error("Invalid timezone", k400BadRequest));
        return;
    }
    if (!Utils::is_valid_input(passengers, passengersPattern)) {
        callback(Utils::error("Invalid passenger count", k400BadRequest));
        return;
    }

    orm::DbClientPtr dbClient = drogon::app().getDbClient("main");
    dbClient->execSqlAsync(
        "SELECT id, departure_airport_id, arrival_airport_id, "
        "base_ticket_price, flight_time, departure + (FLOOR(date_part('epoch', "
        "(DATE($3 AT TIME ZONE $4)::timestamp - (departure AT TIME ZONE "
        "$4)::date::timestamp))/date_part('epoch', frequency)) * frequency) AS "
        "departure FROM flight WHERE departure_airport_id = $1 AND "
        "arrival_airport_id = $2 AND (departure AT TIME ZONE $4)::date <= "
        "DATE($3 AT TIME ZONE $4) AND MOD(date_part('epoch', (DATE($3 AT TIME "
        "ZONE $4)::timestamp - (departure AT TIME ZONE "
        "$4)::date::timestamp))::bigint,date_part('epoch', frequency)::bigint) "
        "= 0;",
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
                jsonResponse.append(rowResult);
            }
            callback(HttpResponse::newHttpJsonResponse(jsonResponse));
        },
        [callback](const drogon::orm::DrogonDbException& e) {
            callback(Skybridge::Utils::error("Database error",
                                             k500InternalServerError,
                                             Json::Value(e.base().what())));
        },
        origin, destination, departure_date, timezone);
}

void search::airports(const HttpRequestPtr& req,
                      std::function<void(const HttpResponsePtr&)>&& callback) {
    orm::DbClientPtr dbClient = drogon::app().getDbClient("main");
    dbClient->execSqlAsync(
        "SELECT name, id, country, city FROM airport",
        [callback](const drogon::orm::Result& result) {
            Json::Value jsonResponse;
            for (const orm::Row& row : result) {
                Json::Value rowResult;
                rowResult["id"] = row["id"].as<std::string>();
                rowResult["name"] = row["name"].as<std::string>();
                rowResult["place"] = row["city"].as<std::string>() + ", " +
                                     row["country"].as<std::string>();
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

static const std::regex airplanePattern("^SB-[A-Z][0-9]{4}$");
// TODO: do search sorting and filtering options
// (such as sort for price, flight length, etc, filter for x class only)
void search::seatmap(const HttpRequestPtr& req,
                     std::function<void(const HttpResponsePtr&)>&& callback) {
    // sanity checking boilerplate
    const auto& parameters = req->getParameters();
    std::string airplane = req->getParameter("airplane");
    if (!Utils::is_valid_input(airplane, airplanePattern)) {
        callback(Utils::error("Invalid airplane.", k400BadRequest));
        return;
    }

    orm::DbClientPtr dbClient = drogon::app().getDbClient("main");
    dbClient->execSqlAsync(
        "SELECT (SELECT seatmap FROM airplane WHERE id = $1) AS seatmap, "
        "(SELECT array_agg(seat_id) AS seats FROM booking WHERE flight_id = "
        "$1) AS occupied_seats",
        [callback](const drogon::orm::Result& result) {
            Json::Value jsonResponse =
                Utils::parseJsonField(result[0]["seatmap"].as<std::string>());

            std::vector<std::string> seats = Utils::parsePgArray(
                result[0]["occupied_seats"].as<std::string>());
            Json::Value seatsJson(Json::arrayValue);
            for (const std::basic_string<char>& seat : seats) {
                seatsJson.append(seat);
            }
            
            jsonResponse["occupied_seats"] = seatsJson;
            callback(HttpResponse::newHttpJsonResponse(jsonResponse));
        },
        [callback](const drogon::orm::DrogonDbException& e) {
            callback(Skybridge::Utils::error("Database error",
                                             k500InternalServerError,
                                             Json::Value(e.base().what())));
        },
        airplane);
}