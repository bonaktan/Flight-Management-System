#include "api_booking.h"

#include "../utils/utils.h"

using namespace api;

// Add definition of your processing function here
Task<HttpResponsePtr> booking::submit(HttpRequestPtr req) {
    std::shared_ptr<Json::Value> json = req->getJsonObject();
    if (!json) {
        co_return Skybridge::Utils::error("Invalid JSON", k400BadRequest);
    }

    std::vector<std::string> errors = Skybridge::Utils::validateRequest(
        *json, booking::create_booking_schema());
    if (!errors.empty()) {
        Json::Value body;
        for (std::string& e : errors) body["details"].append(e);
        co_return Skybridge::Utils::error("Validation failed", k400BadRequest,
                                          body["details"]);
    }

    std::string account_id = req->attributes()->get<std::string>("userId");
    std::string flight_id = (*json)["flightId"].asString();
    const Json::Value& passengers = (*json)["passengers"];
    std::string departure_date = (*json)["departure_date"].asString();
    int total = passengers.size();
    auto completed = std::make_shared<std::atomic<int>>(0);
    auto failed = std::make_shared<std::atomic<bool>>(false);

    orm::DbClientPtr db = drogon::app().getDbClient("main");
    auto transPtr = co_await db->newTransactionCoro();
    try {
        std::vector<long long> passengerIds;

        for (const Json::Value& p : passengers) {
            auto r = co_await transPtr->execSqlCoro(
                "INSERT INTO passenger (title, first_name, middle_name, "
                "last_name, gender, birthdate, "
                "contact_email, phone_number, emergency_contact_name, "
                "emergency_contact_phone, associated_to) "
                "VALUES ($1,$2,$3,$4,$5,$6,$7,$8,$9,$10,$11) RETURNING id",
                p["title"].asString(), p["first_name"].asString(),
                p["middle_name"].asString(), p["last_name"].asString(),
                p["gender"].asString(), p["date_of_birth"].asString(),
                p["email"].asString(), p["phone_number"].asString(),
                p["emergency_contact_name"].asString(),
                p["emergency_phone_number"].asString(), account_id);
            passengerIds.push_back(r[0]["id"].as<long long>());
        }
        
        auto r2 = co_await transPtr->execSqlCoro(
            "INSERT INTO booking (flight_id, account_id, payment_option, "
            "payment_detail, booking_status, departure_date) "
            "VALUES ($1,$2,$3,$4,$5,$6) RETURNING id",
            flight_id, account_id, "Online Banking", "{}", "accepted",
            departure_date);
        long long bookingId = r2[0]["id"].as<long long>();

        for (int i = 0; i < (int)passengerIds.size(); ++i) {
            co_await transPtr->execSqlCoro(
                "INSERT INTO booking_passenger (booking_id, passenger_id, "
                "seat_id, calculated_price) "
                "VALUES ($1,$2,$3,$4)",
                bookingId, passengerIds[i], passengers[i]["selected_seat"].asString(),
                (double)6969.69);
        }

    } catch (const drogon::orm::DrogonDbException& e) {
        LOG_ERROR << "Transaction failed: " << e.base().what();
        transPtr->rollback();
        co_return Skybridge::Utils::error(
            "Database Error", drogon::k500InternalServerError, e.base().what());
    }

    Json::Value jsonResponse;
    jsonResponse["success"] = true;
    co_return HttpResponse::newHttpJsonResponse(jsonResponse);
}

