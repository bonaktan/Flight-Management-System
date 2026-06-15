#include <valijson/adapters/jsoncpp_adapter.hpp>
#include <valijson/schema.hpp>
#include <valijson/schema_parser.hpp>
#include <valijson/validator.hpp>

#include "../controllers/api_admin.h"

const valijson::Schema& api::admin::update_schema() {
    static valijson::Schema schema = [] {
        Json::Value s;
        s["$schema"] = "http://json-schema.org/draft-07/schema#";
        s["type"] = "object";

        s["required"].append("field");
        s["required"].append("value");
        s["additionalProperties"] = false;

        Json::Value& p = s["properties"];

        p["field"]["type"] = "string";
        p["field"]["minLength"] = 2;
        p["field"]["maxLength"] = 127;

        p["value"]["type"] = "string";
        p["value"]["minLength"] = 0;
        p["value"]["maxLength"] = 2048;

        valijson::Schema out;
        valijson::SchemaParser parser;
        valijson::adapters::JsonCppAdapter adapter(s);
        parser.populateSchema(adapter, out);
        return out;
    }();
    return schema;
}
