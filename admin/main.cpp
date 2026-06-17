
#include "main.h"

#include "./api/api.h"
#include "./display/display.h"
int main(int argc, char* argv[]) {
    Skybridge::API::ApiClient& auth = Skybridge::API::ApiClient::getInstance();
    auth.base_url = "https://skybridge.bonnybonnybonaktan.xyz/api";

    // Check if --dev flag exists
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--dev") {
            auth.base_url = "http://localhost:8080/api";
            break;
        }
    }

    if (!Skybridge::Menu::authenticate()) return 1;
    Skybridge::Display::pause();
    Skybridge::Menu::mainMenu();
    return 0;
}