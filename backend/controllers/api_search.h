#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

namespace api
{
class search : public drogon::HttpController<search>
{
  public:
    METHOD_LIST_BEGIN
    METHOD_ADD(search::flights, "/flights", Get);
    METHOD_ADD(search::airports, "/airports", Get);
    METHOD_ADD(search::airplanes, "/airplanes", Get);
    METHOD_LIST_END
    void flights(const HttpRequestPtr& req,
                 std::function<void(const HttpResponsePtr&)>&& callback);
    void airports(const HttpRequestPtr& req,
                   std::function<void(const HttpResponsePtr&)>&& callback);
    void airplanes(const HttpRequestPtr& req,
                     std::function<void(const HttpResponsePtr&)>&& callback);
   private:
    const valijson::Schema& flight_schema();
};
}
