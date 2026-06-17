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
    METHOD_ADD(search::airports, "/airports", Get);
    METHOD_ADD(search::seatmap, "/airplane/seatmap", Get);
    METHOD_ADD(search::flightById, "/flight/{id}", Get);
    METHOD_LIST_END
    void flights(const HttpRequestPtr& req,
                 std::function<void(const HttpResponsePtr&)>&& callback);
    void airports(const HttpRequestPtr& req,
                  std::function<void(const HttpResponsePtr&)>&& callback);
    void seatmap(const HttpRequestPtr& req,
                 std::function<void(const HttpResponsePtr&)>&& callback);
    void flightById(const HttpRequestPtr& req,
                    std::function<void(const HttpResponsePtr&)>&& callback,
                    std::string flightId);

   private:
    const valijson::Schema& flight_schema();
};
}  // namespace api
