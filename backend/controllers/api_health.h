#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

namespace api
{
class health : public drogon::HttpController<health>
{
  public:
    METHOD_LIST_BEGIN
    METHOD_ADD(health::healthcheck, "", Get);
    METHOD_ADD(health::healthcheck, "/", Get);
    METHOD_LIST_END

    void healthcheck(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
};
}
