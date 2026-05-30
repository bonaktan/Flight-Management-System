#include "api_search.h"

using namespace api;

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
        "SELECT * FROM flight;",
        [callback](const drogon::orm::Result& result) {
            Json::Value jsonResponse;
            
            for (const orm::Row& row : result) {
                Json::Value rowResult;
                rowResult["id"] = row["id"].as<std::string>();
                rowResult["departure_airport_id"] = row["departure_airport_id"].as<std::string>();
                rowResult["arrival_airport_id"] = row["arrival_airport_id"].as<std::string>();
                rowResult["base_ticket_price"] = row["base_ticket_price"].as<double>();
                rowResult["flight_time"] = row["flight_time"].as<std::string>();
                rowResult["departure"] = row["departure"].as<std::string>();
                rowResult["frequency"] = row["frequency"].as<std::string>();
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
        "SELECT name, id, country, city FROM airport;",
        [callback](const drogon::orm::Result& result) {
            Json::Value jsonResponse;

            for (const orm::Row& row : result) {
                Json::Value rowResult;
                rowResult["name"] = row["name"].as<std::string>();
                rowResult["id"] = row["id"].as<std::string>();
                rowResult["country"] = row["country"].as<std::string>();
                rowResult["city"] = row["city"].as<std::string>();
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

void search::airplanes(const HttpRequestPtr& req,
               std::function<void(const HttpResponsePtr&)>&& callback) {
    orm::DbClientPtr dbClient = drogon::app().getDbClient("main");
    dbClient->execSqlAsync(
        "SELECT a.id AS airplane_id, a.model, a.location, MAX(CASE WHEN sc.rn "
        "= 1 THEN sc.class_name END) AS class_1_name,MAX(CASE WHEN sc.rn = "
        "1 THEN sc.amt_of_seats END) AS class_1_amt_of_seats, MAX(CASE WHEN "
        "sc.rn = 1 THEN sc.markup_price  END) AS class_1_markup_price, "
        "MAX(CASE WHEN sc.rn = 2 THEN sc.class_name END) AS class_2_name, "
        "MAX(CASE WHEN sc.rn = 2 THEN sc.amt_of_seats END) AS "
        "class_2_amt_of_seats, MAX(CASE WHEN sc.rn = 2 THEN sc.markup_price  "
        "END) AS class_2_markup_price, MAX(CASE WHEN sc.rn = 3 THEN "
        "sc.class_name END) AS class_3_name, MAX(CASE WHEN sc.rn = 3 THEN "
        "sc.amt_of_seats END) AS class_3_amt_of_seats, MAX(CASE WHEN sc.rn = "
        "3 THEN sc.markup_price  END) AS class_3_markup_price FROM airplane a "
        "LEFT JOIN (SELECT *, ROW_NUMBER() OVER (PARTITION BY airplane_id "
        "ORDER BY id) AS rn FROM seat_class) sc ON sc.airplane_id = a.id "
        "GROUP BY a.id, a.model, a.location;",
        [callback](const drogon::orm::Result& result) {
            Json::Value jsonResponse;
            for (const orm::Row& row : result) {
                Json::Value rowResult;
                rowResult["airplane_id"] = row["airplane_id"].as<std::string>();
                rowResult["model"] = row["model"].as<std::string>();
                rowResult["location"] = row["location"].as<std::string>();
                rowResult["class_1_name"] =
                    row["class_1_name"].as<std::string>();
                rowResult["class_1_amt_of_seats"] =
                    row["class_1_amt_of_seats"].as<std::string>();
                rowResult["class_1_markup_price"] =
                    row["class_1_markup_price"].as<std::string>();
                rowResult["class_2_name"] =
                    row["class_2_name"].as<std::string>();
                rowResult["class_2_amt_of_seats"] =
                    row["class_2_amt_of_seats"].as<std::string>();
                rowResult["class_2_markup_price"] =
                    row["class_2_markup_price"].as<std::string>();
                rowResult["class_3_name"] =
                    row["class_3_name"].as<std::string>();
                rowResult["class_3_amt_of_seats"] =
                    row["class_3_amt_of_seats"].as<std::string>();
                rowResult["class_3_markup_price"] =
                    row["class_3_markup_price"].as<std::string>();
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