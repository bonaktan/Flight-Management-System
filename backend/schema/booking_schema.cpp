#include <valijson/adapters/jsoncpp_adapter.hpp>
#include <valijson/schema.hpp>
#include <valijson/schema_parser.hpp>
#include <valijson/validator.hpp>

#include "../controllers/api_booking.h"

const valijson::Schema& api::booking::create_booking_schema() {
    static valijson::Schema schema = [] {
        Json::Value s;
        s["$schema"] = "http://json-schema.org/draft-07/schema#";
        s["type"]    = "object";
        s["required"].append("flightId");
        s["required"].append("passengers");
        s["required"].append("departure_date");
        s["additionalProperties"] = false;

        Json::Value& p = s["properties"];

        // flightId
        p["flightId"]["type"] = "string";
        p["flightId"]["pattern"] = "^[A-Z]{3}[0-9]{3}$";

        // passengers array
        p["passengers"]["type"] = "array";
        p["passengers"]["minItems"] = 1;
        p["passengers"]["maxItems"] = 9;

        p["departure_date"]["type"] = "string";
        p["departure_date"]["pattern"] = "^[0-9]{4}-(0[1-9]|1[0-2])-(0[1-9]|[12][0-9]|3[01])$";

        // passenger item schema
        Json::Value& item = p["passengers"]["items"];
        item["type"] = "object";
        item["additionalProperties"] = false;

        item["required"].append("title");
        item["required"].append("first_name");
        item["required"].append("middle_name");
        item["required"].append("last_name");
        item["required"].append("gender");
        item["required"].append("date_of_birth");
        item["required"].append("email");
        item["required"].append("phone_number");
        item["required"].append("emergency_contact_name");
        item["required"].append("emergency_phone_number");
        item["required"].append("selected_seat");

        Json::Value& ip = item["properties"];

        ip["title"]["type"] = "string";
        ip["title"]["enum"].append("mr");
        ip["title"]["enum"].append("ms");
        ip["title"]["enum"].append("mrs");
        ip["title"]["enum"].append("dr");

        ip["first_name"]["type"] = "string";
        ip["first_name"]["minLength"] = 1;
        ip["first_name"]["maxLength"] = 64;

        ip["middle_name"]["type"] = "string";
        ip["middle_name"]["maxLength"] = 64;

        ip["last_name"]["type"] = "string";
        ip["last_name"]["minLength"] = 1;
        ip["last_name"]["maxLength"] = 64;

        ip["gender"]["type"] = "string";
        ip["gender"]["enum"].append("male");
        ip["gender"]["enum"].append("female");

        ip["date_of_birth"]["type"] = "string";
        ip["date_of_birth"]["pattern"] = "^[0-9]{4}-(0[1-9]|1[0-2])-(0[1-9]|[12][0-9]|3[01])$";

        ip["email"]["type"] = "string";
        ip["email"]["format"] = "email";
        ip["email"]["maxLength"] = 254;

        ip["phone_number"]["type"] = "string";
        ip["phone_number"]["pattern"] = "^[0-9]{10,15}$";

        ip["emergency_contact_name"]["type"] = "string";
        ip["emergency_contact_name"]["minLength"] = 1;
        ip["emergency_contact_name"]["maxLength"] = 128;

        ip["emergency_phone_number"]["type"] = "string";
        ip["emergency_phone_number"]["pattern"] = "^[0-9]{10,15}$";

        ip["selected_seat"]["type"] = "string";
        ip["selected_seat"]["pattern"] = "^[A-Z][0-9]{1,2}$";

        valijson::Schema out;
        valijson::SchemaParser parser;
        valijson::adapters::JsonCppAdapter adapter(s);
        parser.populateSchema(adapter, out);
        return out;
    }();
    return schema;
}