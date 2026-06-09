#include "api_account.h"

#include "../utils/utils.h"
using namespace api;

// Add definition of your processing function here
void account::details(const HttpRequestPtr& req,
                      std::function<void(const HttpResponsePtr&)>&& callback) {
    std::string userId = req->attributes()->get<std::string>("userId");
    orm::DbClientPtr dbClient = drogon::app().getDbClient("main");
    dbClient->execSqlAsync(
        "SELECT account_name, email FROM account WHERE id=$1",
        [callback, userId](const drogon::orm::Result& result) {
            Json::Value jsonResponse;
            jsonResponse["email"] = result[0]["email"].as<std::string>();
            jsonResponse["userId"] = userId;
            jsonResponse["account_name"] =
                result[0]["account_name"].as<std::string>();
            callback(HttpResponse::newHttpJsonResponse(jsonResponse));
        },
        [callback](const drogon::orm::DrogonDbException& e) {
            callback(Skybridge::Utils::error(
                "Database Error.", k500InternalServerError, e.base().what()));
        },
        userId);
}

void account::bookings(const HttpRequestPtr& req,
                       std::function<void(const HttpResponsePtr&)>&& callback) {
    std::string userId = req->attributes()->get<std::string>("userId");
    orm::DbClientPtr dbClient = drogon::app().getDbClient("main");
    dbClient->execSqlAsync(  // WARN: no roundtrip flight support tong sql
                             // lookup na to
        "WITH user_bookings AS (SELECT booking.id, booking.flight_id, "
        "booking_passenger.passenger_id, booking_passenger.seat_id, "
        "booking_passenger.calculated_price, booking.departure_date FROM "
        "booking LEFT JOIN booking_passenger ON booking.id = "
        "booking_passenger.booking_id WHERE account_id = $1) SELECT "
        "user_bookings.flight_id, user_bookings.calculated_price, "
        "(user_bookings.departure_date::DATE + "
        "flight.departure::TIME)::timestamp AS departure, EXTRACT(EPOCH FROM "
        "flight.flight_time)::int AS flight_time, flight.departure_airport_id, "
        "flight.arrival_airport_id, flight.airplane_id, airplane.model, "
        "JSON_AGG(JSON_BUILD_OBJECT('seat', user_bookings.seat_id, 'title', "
        "passenger.title, 'name', (passenger.first_name || ' ' || "
        "COALESCE(passenger.middle_name || ' ', '') || passenger.last_name))) "
        "AS passengers FROM user_bookings LEFT JOIN passenger ON "
        "user_bookings.passenger_id = passenger.id LEFT JOIN flight ON "
        "user_bookings.flight_id = flight.id LEFT JOIN airplane ON "
        "flight.airplane_id = airplane.id GROUP BY user_bookings.id, "
        "user_bookings.flight_id, user_bookings.calculated_price, "
        "user_bookings.departure_date, flight.flight_time, "
        "flight.departure_airport_id, flight.arrival_airport_id, "
        "flight.airplane_id, flight.departure, airplane.model",
        [callback, userId](const drogon::orm::Result& result) {
            Json::Value jsonResponse;
            for (const orm::Row& row : result) {
                Json::Value rowResult;
                Json::Value departureFlight;
                rowResult["isRoundTrip"] = false;  // TODO : roundtrip support
                departureFlight["flightId"] =
                    row["flight_id"].as<std::string>();
                departureFlight["departure"] =
                    row["departure"].as<std::string>();
                departureFlight["flight_time"] = row["flight_time"].as<int>();
                departureFlight["origin"] =
                    row["departure_airport_id"].as<std::string>();
                departureFlight["destination"] =
                    row["arrival_airport_id"].as<std::string>();
                departureFlight["airplane_id"] =
                    row["airplane_id"].as<std::string>();
                departureFlight["model"] = row["model"].as<std::string>();
                rowResult["departureFlight"] = departureFlight;
                rowResult["passengers"] = Skybridge::Utils::parseJsonField(
                    row["passengers"].as<std::string>());
                rowResult["price"] = row["calculated_price"].as<std::string>();
                jsonResponse.append(rowResult);
            }
            callback(HttpResponse::newHttpJsonResponse(jsonResponse));
        },
        [callback](const drogon::orm::DrogonDbException& e) {
            callback(Skybridge::Utils::error(
                "Database Error.", k500InternalServerError, e.base().what()));
        },
        userId);
}