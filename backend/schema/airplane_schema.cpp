#include <valijson/adapters/jsoncpp_adapter.hpp>
#include <valijson/schema.hpp>
#include <valijson/schema_parser.hpp>
#include <valijson/validator.hpp>

#include "../controllers/api_admin.h"

const valijson::Schema& api::admin::add_airplane_schema() {
    static valijson::Schema schema = [] {
        Json::Value s;
        s["$schema"] = "http://json-schema.org/draft-07/schema#";
        s["type"]    = "object";

        s["required"].append("airplane_id");
        s["required"].append("model");
        s["required"].append("location");
        s["additionalProperties"] = false;

        Json::Value& p = s["properties"];

        p["airplane_id"]["type"] = "string";
        p["airplane_id"]["pattern"] = "^SB-[A-Z][0-9]{4}$";

        p["model"]["type"] = "string";
        p["model"]["maxLength"] = 127;

        p["location"]["type"] = "string";
        p["location"]["pattern"] = "^[A-Z]{3}$";


        valijson::Schema out;
        valijson::SchemaParser parser;
        valijson::adapters::JsonCppAdapter adapter(s);
        parser.populateSchema(adapter, out);
        return out;
    }();
    return schema;
}


