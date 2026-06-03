#include <valijson/adapters/jsoncpp_adapter.hpp>
#include <valijson/schema.hpp>
#include <valijson/schema_parser.hpp>
#include <valijson/validator.hpp>

#include "../controllers/api_admin.h"

const valijson::Schema& api::admin::add_flight_schema() {
    static valijson::Schema schema = [] {
        Json::Value s;
        s["$schema"] = "http://json-schema.org/draft-07/schema#";
        s["type"]    = "object";

        s["required"].append("flight_id");
        s["required"].append("departure_airport_id");
        s["required"].append("arrival_airport_id");
        s["required"].append("base_ticket_price");
        s["required"].append("flight_time");
        s["required"].append("start_of_operations");
        s["required"].append("frequency");
        s["additionalProperties"] = false;

        Json::Value& p = s["properties"];

        p["flight_id"]["type"]        = "string";
        p["flight_id"]["pattern"]     = "^SKY[0-9]{3}$";

        p["departure_airport_id"]["type"] = "string";
        p["arrival_airport_id"]["type"]   = "string";
        p["departure_airport_id"]["pattern"]     = "^[A-Z]{3}$";
        p["arrival_airport_id"]["pattern"]     = "^[A-Z]{3}$";

        p["base_ticket_price"]["type"] = "number";
        p["base_ticket_price"]["minimum"] = 0;

        p["flight_time"]["type"] = "string";
        p["flight_time"]["pattern"] = "[0-9]{2}:[0-9]{2}:[0-9]{2}";

        p["start_of_operations"]["type"] = "string";
        p["start_of_operations"]["format"] = "date-time";

        p["frequency"]["type"] = "string";
        p["frequency"]["pattern"] = "^P(?!$)(?:\\d+Y)?(?:\\d+M)?(?:\\d+W)?(?:\\d+D)?(?:T(?=\\d)(?:\\d+H)?(?:\\d+M)?(?:\\d+(?:\\.\\d+)?S)?)?$";

        p["passengers"]["type"]    = "integer";
        p["passengers"]["minimum"] = 1;
        p["passengers"]["maximum"] = 9;

        valijson::Schema out;
        valijson::SchemaParser parser;
        valijson::adapters::JsonCppAdapter adapter(s);
        parser.populateSchema(adapter, out);
        return out;
    }();
    return schema;
}


