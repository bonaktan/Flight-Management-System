#include <valijson/adapters/jsoncpp_adapter.hpp>
#include <valijson/schema.hpp>
#include <valijson/schema_parser.hpp>
#include <valijson/validator.hpp>

#include "../controllers/api_admin.h"

const valijson::Schema& api::admin::add_airport_schema() {
    static valijson::Schema schema = [] {
        Json::Value s;
        s["$schema"] = "http://json-schema.org/draft-07/schema#";
        s["type"]    = "object";

        s["required"].append("name");
        s["required"].append("airport_id");
        s["required"].append("capacity");
        s["required"].append("country");
        s["required"].append("city");
        s["additionalProperties"] = false;

        Json::Value& p = s["properties"];

        p["name"]["type"] = "string";
        p["name"]["minLength"] = 2;
        p["name"]["maxLength"] = 127;

        p["airport_id"]["type"] = "string";
        p["airport_id"]["pattern"] = "^[A-Z]{3}$";

        p["capacity"]["type"] = "integer";
        p["capacity"]["minimum"] = 1;

        p["country"]["type"] = "string";
        p["country"]["minLength"] = 2;
        p["country"]["maxLength"] = 64;

        p["city"]["type"] = "string";
        p["city"]["minLength"] = 2;
        p["city"]["maxLength"] = 127;


        valijson::Schema out;
        valijson::SchemaParser parser;
        valijson::adapters::JsonCppAdapter adapter(s);
        parser.populateSchema(adapter, out);
        return out;
    }();
    return schema;
}


