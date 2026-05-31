#include <drogon/drogon.h>

int main() {
    drogon::app().loadConfigFile("config.json");

    // Global CORS handling
    drogon::app().registerPreRoutingAdvice(
        [](const drogon::HttpRequestPtr& req, drogon::AdviceCallback&& acb,
           drogon::AdviceChainCallback&& accb) {
            if (req->method() == drogon::Options) {
                drogon::HttpResponsePtr resp = drogon::HttpResponse::newHttpResponse();
                resp->setStatusCode(drogon::k204NoContent);
                resp->addHeader("Access-Control-Allow-Origin", "*");
                resp->addHeader("Access-Control-Allow-Methods",
                                "GET, POST, PUT, DELETE, PATCH, OPTIONS");
                resp->addHeader(
                    "Access-Control-Allow-Headers",
                    "Content-Type, Authorization, X-Requested-With");
                resp->addHeader("Access-Control-Max-Age", "86400");
                acb(resp);
                return;
            }
            accb(); 
        });
    drogon::app().registerPostHandlingAdvice(
        [](const drogon::HttpRequestPtr&, const drogon::HttpResponsePtr& resp) {
            resp->addHeader("Access-Control-Allow-Origin", "*");
            resp->addHeader("Access-Control-Allow-Methods",
                            "GET, POST, PUT, DELETE, PATCH, OPTIONS");
            resp->addHeader("Access-Control-Allow-Headers",
                            "Content-Type, Authorization, X-Requested-With");
        });
    
    drogon::app().setTermSignalHandler([]() {
        LOG_INFO << "Shutting down...";
        drogon::app().quit();
    });

    drogon::app().getLoop()->queueInLoop([]() {
        drogon::orm::DbClientPtr dbClient = drogon::app().getDbClient("main");
        try {
            drogon::orm::Result result =
                dbClient->execSqlSync("SELECT version();");
            std::cout << "Database is Ready" << std::endl;
        } catch (drogon::orm::DrogonDbException& e) {
            std::cerr << "error:" << e.base().what() << std::endl;
            // TODO: close the app, you cant live w/o the db :)
        }
    });

    std::cout << "Backend running on http://localhost:8080" << std::endl;
    drogon::app().run();
    return 0;
}
