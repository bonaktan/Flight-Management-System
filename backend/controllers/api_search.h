#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

namespace api
{
class search : public drogon::HttpController<search>
{
  public:
    METHOD_LIST_BEGIN
    // use METHOD_ADD to add your custom processing function here;
    METHOD_ADD(search::flights, "/flights", Get); // path is /api/search/{arg2}/{arg1}
    // METHOD_ADD(search::your_method_name, "/{1}/{2}/list", Get); // path is /api/search/{arg1}/{arg2}/list
    // ADD_METHOD_TO(search::your_method_name, "/absolute/path/{1}/{2}/list", Get); // path is /absolute/path/{arg1}/{arg2}/list

    METHOD_LIST_END
    // your declaration of processing function maybe like this:
    void flights(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback);
    // void your_method_name(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, double p1, int p2) const;
};
}
