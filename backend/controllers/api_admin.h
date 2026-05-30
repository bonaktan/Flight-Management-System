#pragma once

#include <drogon/HttpController.h>
#include <valijson/schema.hpp>
using namespace drogon;

namespace api {
class admin : public drogon::HttpController<admin> {
   public:
    METHOD_LIST_BEGIN
    METHOD_ADD(admin::dashboard, "/dashboard", Get);
    METHOD_ADD(admin::add_airplane, "/airplane/add", Post);
    METHOD_ADD(admin::add_airport, "/airport/add", Post);
    METHOD_ADD(admin::add_flight, "/flight/add", Post);

    METHOD_LIST_END
    void dashboard(const HttpRequestPtr& req,
                   std::function<void(const HttpResponsePtr&)>&& callback);
    void add_flight(const HttpRequestPtr& req,
                     std::function<void(const HttpResponsePtr&)>&& callback);
    void add_airplane(const HttpRequestPtr& req,
                      std::function<void(const HttpResponsePtr&)>&& callback);
    void add_airport(const HttpRequestPtr& req,
                     std::function<void(const HttpResponsePtr&)>&& callback);
private:
        const valijson::Schema& add_flight_schema();
        const valijson::Schema& add_airplane_schema();
        const valijson::Schema& add_airport_schema();
};
}  // namespace api
