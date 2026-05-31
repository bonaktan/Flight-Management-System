#pragma once

#include <drogon/HttpController.h>
// #include <valijson/adapters/jsoncpp_adapter.hpp>
#include <valijson/schema.hpp>
// #include <valijson/schema_parser.hpp>
// #include <valijson/validator.hpp>

using namespace drogon;

namespace api {
class search : public drogon::HttpController<search> {
   public:
    METHOD_LIST_BEGIN
    METHOD_ADD(search::flights, "/flights", Get);
    METHOD_LIST_END
    void flights(const HttpRequestPtr& req,
                 std::function<void(const HttpResponsePtr&)>&& callback);

   private:
    const valijson::Schema& flight_schema();
};
}  // namespace api
