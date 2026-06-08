#pragma once

#include <drogon/HttpController.h>

#include <valijson/schema.hpp>

using namespace drogon;

namespace api {
class booking : public drogon::HttpController<booking> {
   public:
    METHOD_LIST_BEGIN
    METHOD_ADD(booking::submit, "/submit", Post, "AuthFilter");
    METHOD_LIST_END

    Task<HttpResponsePtr> submit(HttpRequestPtr req);

   private:
    const valijson::Schema& create_booking_schema();
};
}  // namespace api
