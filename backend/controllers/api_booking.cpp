#include "api_booking.h"

#include "../utils/utils.h"

using namespace api;

// Add definition of your processing function here
void booking::submit(const HttpRequestPtr& req,
                     std::function<void(const HttpResponsePtr&)>&& callback) {
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
    auto completed = std::make_shared<std::atomic<int>>(0);
    auto failed = std::make_shared<std::atomic<bool>>(false);

    orm::DbClientPtr db = drogon::app().getDbClient("main");
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
            [completed, total, callback,
             failed](const drogon::orm::Result& res) {
                if (++(*completed) == total && !(*failed)) {
                    Json::Value resp;
                    resp["message"] = "All bookings created";
                    auto r = HttpResponse::newHttpJsonResponse(resp);
                    r->setStatusCode(k200OK);
                    callback(r);
                }
            },
            [completed, total, callback, failed,
             p](const drogon::orm::DrogonDbException& e) {
                if (!failed->exchange(true)) {
                    std::string errMsg = e.base().what();

                    if (errMsg.find("unique_seat_per_flight_date") !=
                        std::string::npos) {
                        Json::Value detail;
                        detail["seat"] = p["selected_seat"].asString();
                        detail["passenger"] = p["first_name"].asString() + " " +
                                              p["last_name"].asString();

                        callback(Skybridge::Utils::error(
                            "Seat " + p["selected_seat"].asString() +
                                " is already taken for this flight.",
                            k409Conflict, detail.toStyledString()));
                    } else {
                        callback(Skybridge::Utils::error(
                            "Database Error.",
                            k500InternalServerError, e.base().what()));
                    }
                }
            },
            p["title"].asString(), p["first_name"].asString(),
            p["middle_name"].asString(), p["last_name"].asString(),
            p["gender"].asString(), p["date_of_birth"].asString(),
            p["email"].asString(), p["phone_number"].asString(),
            p["emergency_contact_name"].asString(),
            p["emergency_phone_number"].asString(), account_id,
            (*json)["flightId"].asString(), 6969.69, "Online Bank", "{}",
            p["selected_seat"].asString(),
            (*json)["departure_date"].asString());
    }
}