#include <valijson/adapters/jsoncpp_adapter.hpp>
#include <valijson/schema.hpp>
#include <valijson/schema_parser.hpp>
#include <valijson/validator.hpp>

#include "../controllers/api_admin.h"

auto make_seat_numbering = [](Json::Value& p_target) {
    p_target["type"] = "object";
    p_target["required"].append("pattern");
    p_target["required"].append("count");
    p_target["additionalProperties"] = false;

    // pattern: array of (string | array-of-strings)
    p_target["properties"]["pattern"]["type"] = "array";
    Json::Value& pattern_items = p_target["properties"]["pattern"]["items"];
    pattern_items["oneOf"][0]["type"] = "string";
    pattern_items["oneOf"][1]["type"] = "array";
    pattern_items["oneOf"][1]["items"]["type"] = "string";

    // count: array of positive integers
    p_target["properties"]["count"]["type"] = "array";
    p_target["properties"]["count"]["items"]["type"] = "integer";
    p_target["properties"]["count"]["items"]["minimum"] = 1;
};

const valijson::Schema& api::admin::add_airplane_schema() {
    static valijson::Schema schema = [] {
        Json::Value s;
        s["$schema"] = "http://json-schema.org/draft-07/schema#";
        s["type"] = "object";

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

        Json::Value& sm = p["seatmap"];
        sm["type"] = "object";
        sm["required"].append("aisleCount");
        sm["required"].append("seatNumbering");
        sm["required"].append("zones");
        sm["additionalProperties"] = false;

        sm["properties"]["aisleCount"]["type"] = "integer";
        sm["properties"]["aisleCount"]["minimum"] = 0;

        make_seat_numbering(sm["properties"]["seatNumbering"]);

        sm["properties"]["occupiedSeats"]["type"] = "array";
        sm["properties"]["occupiedSeats"]["items"]["type"] = "string";

        Json::Value& zones = p["seatmap"]["properties"]["zones"];
        zones["type"] = "array";
        Json::Value& zone_item = zones["items"];

        // --- infra zone ---
        Json::Value infra;
        infra["type"] = "object";
        infra["required"].append("type");
        infra["required"].append("label");
        infra["additionalProperties"] = false;
        infra["properties"]["type"]["type"] = "string";
        infra["properties"]["type"]["enum"].append("infra");
        infra["properties"]["label"]["type"] = "string";
        infra["properties"]["emergencyOnly"]["type"] = "boolean";

        // --- seat zone ---
        Json::Value seat;
        seat["type"] = "object";
        seat["required"].append("type");
        seat["required"].append("label");
        seat["required"].append("colCount");
        seat["additionalProperties"] = false;
        seat["properties"]["type"]["type"] = "string";
        seat["properties"]["type"]["enum"].append("seat");
        seat["properties"]["label"]["type"] = "string";
        seat["properties"]["colCount"]["type"] = "integer";
        seat["properties"]["colCount"]["minimum"] = 1;
        make_seat_numbering(seat["properties"]["seatNumbering"]);

        zone_item["oneOf"].append(infra);
        zone_item["oneOf"].append(seat);
        sm["properties"]["zones"] = zones;

        valijson::Schema out;
        valijson::SchemaParser parser;
        valijson::adapters::JsonCppAdapter adapter(s);
        parser.populateSchema(adapter, out);
        return out;
    }();
    return schema;
}
