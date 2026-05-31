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
    METHOD_ADD(admin::add_staff, "/staff/add", Post);

    METHOD_ADD(admin::view_airplanes, "/airplane/view", Get);
    METHOD_ADD(admin::view_airports, "/airport/view", Get);
    METHOD_ADD(admin::view_account, "/account/view", Get);
    METHOD_ADD(admin::view_staff, "/staff/view", Get);

    METHOD_ADD(admin::delete_airplane, "/airplane/delete/{id}", Delete);
    METHOD_ADD(admin::delete_airport, "/airport/delete/{id}", Delete);
    METHOD_ADD(admin::delete_account, "/account/delete/{id}", Delete);
    METHOD_ADD(admin::delete_staff, "/staff/delete/{id}", Delete);
    METHOD_ADD(admin::delete_flight, "/flight/delete/{id}", Delete);
    METHOD_LIST_END

    void dashboard(const HttpRequestPtr& req,
                   std::function<void(const HttpResponsePtr&)>&& callback);
    void add_flight(const HttpRequestPtr& req,
                    std::function<void(const HttpResponsePtr&)>&& callback);
    void add_airplane(const HttpRequestPtr& req,
                      std::function<void(const HttpResponsePtr&)>&& callback);
    void add_airport(const HttpRequestPtr& req,
                     std::function<void(const HttpResponsePtr&)>&& callback);
    void add_staff(const HttpRequestPtr& req,
                   std::function<void(const HttpResponsePtr&)>&& callback);

    void view_account(const HttpRequestPtr& req,
                      std::function<void(const HttpResponsePtr&)>&& callback);
    void view_staff(const HttpRequestPtr& req,
                    std::function<void(const HttpResponsePtr&)>&& callback);
    void view_airports(const HttpRequestPtr& req,
                       std::function<void(const HttpResponsePtr&)>&& callback);
    void view_airplanes(const HttpRequestPtr& req,
                        std::function<void(const HttpResponsePtr&)>&& callback);

    void delete_airplane(const HttpRequestPtr& req,
                         std::function<void(const HttpResponsePtr&)>&& callback,
                         std::string id);
    void delete_airport(const HttpRequestPtr& req,
                        std::function<void(const HttpResponsePtr&)>&& callback,
                        std::string id);
    void delete_account(const HttpRequestPtr& req,
                        std::function<void(const HttpResponsePtr&)>&& callback,
                        std::string id);
    void delete_staff(const HttpRequestPtr& req,
                      std::function<void(const HttpResponsePtr&)>&& callback,
                      std::string id);
    void delete_flight(const HttpRequestPtr& req,
                       std::function<void(const HttpResponsePtr&)>&& callback,
                       std::string id);

   private:
    const valijson::Schema& add_flight_schema();
    const valijson::Schema& add_airplane_schema();
    const valijson::Schema& add_airport_schema();
    const valijson::Schema& add_staff_schema();
};
}  // namespace api
