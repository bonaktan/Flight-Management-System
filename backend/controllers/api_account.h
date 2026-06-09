#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

namespace api
{
class account : public drogon::HttpController<account>
{
  public:
    METHOD_LIST_BEGIN
    METHOD_ADD(account::details, "/details", Get, "AuthFilter"); 
    METHOD_ADD(account::bookings, "/bookings", Get, "AuthFilter"); 
    METHOD_LIST_END

    void details(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    void bookings(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
};
}
