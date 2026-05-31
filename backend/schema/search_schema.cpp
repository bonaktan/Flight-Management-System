#include <valijson/adapters/jsoncpp_adapter.hpp>
#include <valijson/schema.hpp>
#include <valijson/schema_parser.hpp>
#include <valijson/validator.hpp>

#include "../controllers/api_search.h"

const valijson::Schema& api::search::flightSchema() {
    static valijson::Schema schema = [] {
        Json::Value s;
        s["$schema"] = "http://json-schema.org/draft-07/schema#";
        s["type"]    = "object";

        s["required"].append("origin");
        s["required"].append("destination");
        s["required"].append("departure_date");
        s["required"].append("timezone");
        s["required"].append("passengers");

        s["additionalProperties"] = false;

        Json::Value& p = s["properties"];

        p["origin"]["type"]        = "string";
        p["origin"]["pattern"]     = "^[A-Z]{3}$";
        p["destination"]["type"]   = "string";
        p["destination"]["pattern"]= "^[A-Z]{3}$";

        p["departure_date"]["type"]    = "string";
        p["departure_date"]["pattern"] = "^\\d{4}-\\d{2}-\\d{2}$";
        p["timezone"]["type"]          = "string";
        p["timezone"]["pattern"]       = "^[A-Za-z_]+(?:\\/[A-Za-z_+-]+)+$";
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