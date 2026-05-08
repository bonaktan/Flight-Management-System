#include <drogon/drogon.h>

int main() {
  drogon::app()
      .registerHandler(
          "/hello",
          [](const drogon::HttpRequestPtr&,
             std::function<void(const drogon::HttpResponsePtr&)>&& callback) {
            Json::Value body;
            body["message"] = "Hello from Drogon on VS 2026";
            auto r = drogon::HttpResponse::newHttpJsonResponse(body);
            callback(r);
          },
          {drogon::Get})
      .addListener("0.0.0.0", 8080)
      .run();
}
