#include <drogon/drogon.h>
int main() {
    drogon::app().loadConfigFile("config.json");
    drogon::app().setTermSignalHandler([]() {
        LOG_INFO << "Shutting down...";
        drogon::app().quit();
    });

    drogon::app().getLoop()->queueInLoop([]() {
        drogon::orm::DbClientPtr dbClient = drogon::app().getDbClient("main");
        try {
            drogon::orm::Result result = dbClient->execSqlSync("SELECT version();");
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
