#include <drogon/drogon.h>
int main() {
    // Set HTTP listener address and port
    // drogon::app().addListener("127.0.0.1", 8080);
    // Load config file
    drogon::app().loadConfigFile("./config.json");

    // Test database connection after app starts
    drogon::app().setTermSignalHandler([]() {
        LOG_INFO << "Shutting down...";
        drogon::app().quit();
    });

    // Register a callback to run after the event loop starts
    drogon::app().getLoop()->queueInLoop([]() {
        auto dbClient = drogon::app().getDbClient("main");
        try {
            auto result = dbClient->execSqlSync("SELECT version();");
            std::cout << result.size() << std::endl;
        } catch(drogon::orm::DrogonDbException& e) {
            std::cerr << "error:" << e.base().what() << std::endl;
        }
        // std::cout << result << "\n"
    });

    // drogon::app().loadConfigFile("../config.yaml");
    // Run HTTP framework,the method will block in the internal event loop
    drogon::app().run();
    return 0;
}
