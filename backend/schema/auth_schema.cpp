#include <valijson/adapters/jsoncpp_adapter.hpp>
#include <valijson/schema.hpp>
#include <valijson/schema_parser.hpp>
#include <valijson/validator.hpp>

#include "../controllers/api_auth.h"

const valijson::Schema& api::auth::signup_schema() {
    static valijson::Schema schema = [] {
        Json::Value s;
        s["$schema"] = "http://json-schema.org/draft-07/schema#";
        s["type"] = "object";

        s["required"].append("email");
        s["required"].append("password");
        s["required"].append("name");
        s["additionalProperties"] = false;

        Json::Value& p = s["properties"];

        p["email"]["type"] = "string";
        p["email"]["pattern"] =
            "^(?!\\.)(?!.*\\.\\.)([a-z0-9_'+\\-\\.]*)[a-z0-9_+\\-]@([a-z0-9]["
            "a-z0-9\\-]*\\.)+[a-z]{2,}$";

        p["password"]["type"] = "string";
        // p["password"]["minLength"] = 8;
        // p["password"]["maxLength"] = 127;
        p["password"]["pattern"] =
            "^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)(?=.*[@$!%*?&])[A-Za-z\\d@$!%*?&]{"
            "8,127}$";

        p["name"]["type"] = "string";
        p["name"]["minLength"] = 5;
        p["name"]["maxLength"] = 127;

        valijson::Schema out;
        valijson::SchemaParser parser;
        valijson::adapters::JsonCppAdapter adapter(s);
        parser.populateSchema(adapter, out);
        return out;
    }();
    return schema;
}