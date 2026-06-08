#include "api_booking.h"

#include "../utils/utils.h"

using namespace api;

// Add definition of your processing function here
void booking::submit(const HttpRequestPtr& req,
                     std::function<void(const HttpResponsePtr&)>&& callback) {
    // --- JSON validation ---
    std::shared_ptr<Json::Value> json = req->getJsonObject();
    if (!json) {
        callback(Skybridge::Utils::error("Invalid JSON", k400BadRequest));
        return;
    }

    std::vector<std::string> errors = Skybridge::Utils::validateRequest(
        *json, api::booking::create_booking_schema());
    if (!errors.empty()) {
        Json::Value body;
        for (std::string& e : errors) body["details"].append(e);
        callback(Skybridge::Utils::error("Validation failed", k400BadRequest,
                                         body["details"]));
        return;
    }

    std::string account_id = req->attributes()->get<std::string>("userId");
    std::string flight_id = (*json)["flightId"].asString();
    const Json::Value& passengers = (*json)["passengers"];
    int total = passengers.size();

    orm::DbClientPtr db = drogon::app().getDbClient("main");
    // TODO: come back here
    for (int i = 0; i < total; i++) {
        const Json::Value& p = passengers[i];
        double calculated_price = 6767.67;
        db->execSqlAsync(
            "WITH inserted_passenger AS (INSERT INTO passenger (title, "
            "first_name, middle_name, last_name, gender, birthdate, "
            "contact_email, phone_number, emergency_contact_name, "
            "emergency_contact_phone, associated_to) VALUES ($1, $2, "
            "$3, $4, $5, $6, $7, $8, $9, $10, $11) RETURNING id) "
            "INSERT INTO booking (passenger_id, flight_id, account_id, "
            "calculated_price, payment_option, payment_detail, "
            "seat_id, departure_date) SELECT id, $12, $11, $13, $14, "
            "$15, $16, $17 FROM inserted_passenger RETURNING id;",
            [](const drogon::orm::Result& res) {

            },
            [callback](const drogon::orm::DrogonDbException& e) {
                callback(Skybridge::Utils::error("Some bookings failed",
                                                 k500InternalServerError));
            });
    }
}