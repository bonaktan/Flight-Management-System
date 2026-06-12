#pragma once

#include <drogon/HttpController.h>

#include <valijson/schema.hpp>
using namespace drogon;

namespace api {
class admin : public drogon::HttpController<admin> {
   public:
    METHOD_LIST_BEGIN
    METHOD_ADD(admin::dashboard, "/dashboard", Get, "AuthFilter",
               "AdminFilter");

    METHOD_ADD(admin::add_airplane, "/airplane/add", Post, "AuthFilter",
               "AdminFilter");
    METHOD_ADD(admin::add_airport, "/airport/add", Post, "AuthFilter",
               "AdminFilter");
    METHOD_ADD(admin::add_flight, "/flight/add", Post, "AuthFilter",
               "AdminFilter");
    METHOD_ADD(admin::add_staff, "/staff/add", Post, "AuthFilter",
               "AdminFilter");

    METHOD_ADD(admin::view_airplanes, "/airplane/view", Get, "AuthFilter",
               "AdminFilter");
    METHOD_ADD(admin::view_airports, "/airport/view", Get, "AuthFilter",
               "AdminFilter");
    METHOD_ADD(admin::view_account, "/account/view", Get, "AuthFilter",
               "AdminFilter");
    METHOD_ADD(admin::view_staff, "/staff/view", Get, "AuthFilter",
               "AdminFilter");

    METHOD_ADD(admin::view_single_airplane, "/airplane/view/{id}", Get,
               "AuthFilter", "AdminFilter");
    METHOD_ADD(admin::view_single_airport, "/airport/view/{id}", Get,
               "AuthFilter", "AdminFilter");
    METHOD_ADD(admin::view_single_account, "/account/view/{id}", Get,
               "AuthFilter", "AdminFilter");
    METHOD_ADD(admin::view_single_staff, "/staff/view/{id}", Get, "AuthFilter",
               "AdminFilter");

    METHOD_ADD(admin::update_account, "/account/update/{id}", Patch,
               "AuthFilter", "AdminFilter");
    METHOD_ADD(admin::update_airplane, "/airplane/update/{id}", Patch,
               "AuthFilter", "AdminFilter");
    METHOD_ADD(admin::update_airport, "/airport/update/{id}", Patch,
               "AuthFilter", "AdminFilter");
    METHOD_ADD(admin::update_flight, "/flight/update/{id}", Patch, "AuthFilter",
               "AdminFilter");

    METHOD_ADD(admin::delete_airplane, "/airplane/delete/{id}", Delete,
               "AuthFilter", "AdminFilter");
    METHOD_ADD(admin::delete_airport, "/airport/delete/{id}", Delete,
               "AuthFilter", "AdminFilter");
    METHOD_ADD(admin::delete_account, "/account/delete/{id}", Delete,
               "AuthFilter", "AdminFilter");
    METHOD_ADD(admin::delete_staff, "/staff/delete/{id}", Delete, "AuthFilter",
               "AdminFilter");
    METHOD_ADD(admin::delete_flight, "/flight/delete/{id}", Delete,
               "AuthFilter", "AdminFilter");
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

    void view_single_account(
        const HttpRequestPtr& req,
        std::function<void(const HttpResponsePtr&)>&& callback, std::string id);
    void view_single_staff(
        const HttpRequestPtr& req,
        std::function<void(const HttpResponsePtr&)>&& callback, std::string id);
    void view_single_airport(
        const HttpRequestPtr& req,
        std::function<void(const HttpResponsePtr&)>&& callback, std::string id);
    void view_single_airplane(
        const HttpRequestPtr& req,
        std::function<void(const HttpResponsePtr&)>&& callback, std::string id);

    void update_account(const HttpRequestPtr& req,
                        std::function<void(const HttpResponsePtr&)>&& callback,
                        std::string id);
    void update_airplane(const HttpRequestPtr& req,
                         std::function<void(const HttpResponsePtr&)>&& callback,
                         std::string id);
    void update_airport(const HttpRequestPtr& req,
                        std::function<void(const HttpResponsePtr&)>&& callback,
                        std::string id);
    void update_flight(const HttpRequestPtr& req,
                       std::function<void(const HttpResponsePtr&)>&& callback,
                       std::string id);

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
    const valijson::Schema& update_schema();
};
}  // namespace api
