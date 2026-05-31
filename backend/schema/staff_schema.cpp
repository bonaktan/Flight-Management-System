#include <valijson/adapters/jsoncpp_adapter.hpp>
#include <valijson/schema.hpp>
#include <valijson/schema_parser.hpp>
#include <valijson/validator.hpp>

#include "../controllers/api_admin.h"

const valijson::Schema& api::admin::add_staff_schema() {
    static valijson::Schema s = [] {
        Json::Value s;
        s["$schema"] = "http://json-schema.org/draft-07/schema#";
        s["type"] = "object";

        s["required"].append("name");
        s["required"].append("current_location");
        s["required"].append("role");

        s["additionalProperties"] = false;

        Json::Value& p = s["properties"];

        p["name"]["type"] = "string";
        p["name"]["maxLength"] = 127;
        
        p["current_location"]["type"] = "string";
        p["current_location"]["pattern"] = "^[A-Z]{3}$";

        p["role"]["type"] = "string";
        p["role"]["enum"].append("Pilot");
        p["role"]["enum"].append("Flight Attendant");

        valijson::Schema out;
        valijson::SchemaParser parser;
        valijson::adapters::JsonCppAdapter adapter(s);
        parser.populateSchema(adapter, out);
        return out;
    }();
    return s;
}