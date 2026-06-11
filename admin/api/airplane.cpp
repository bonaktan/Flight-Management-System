#include <algorithm>
#include <fstream>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/screen.hpp>
#include <iomanip>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include "../controls/controls.h"
#include "../display/display.h"
#include "./api.h"

using namespace Skybridge;
API::Airplane* API::Airplane::instance = nullptr;
void API::Airplane::save() {
    std::ofstream f(Data::FILE_AIRPLANES);
    for (auto& a : Data::airplanes)
        f << Escape::escape(a.id) << "|" << Escape::escape(a.model) << "|"
          << Escape::escape(a.location) << "\n";
}

void API::Airplane::load() {
    Data::airplanes.clear();
    std::ifstream f(Data::FILE_AIRPLANES);
    std::string line;
    while (std::getline(f, line)) {
        if (line.empty()) continue;
        auto t = Escape::splitLine(line);
        if (t.size() < 3) continue;
        Structs::Airplane a;
        a.id = t[0];
        a.model = t[1];
        a.location = t[2];
        Data::airplanes.push_back(a);
    }
}

std::vector<std::vector<std::string>> API::Airplane::view() {
    API::ApiClient& client = API::ApiClient::getInstance();
    nlohmann::json apiReturn =
        nlohmann::json::parse(client.get("/admin/airplane/view").text);

    std::vector<std::vector<std::string>> data = {{"ID", "Location", "Model"}};
    for (const auto& entry : apiReturn) {
        data.push_back({entry["id"].get<std::string>(),
                        entry["location"].get<std::string>(),
                        entry["model"].get<std::string>()});
    }
    return data;
}

void API::Airplane::add() {
    Display::printHeader("ADD AIRPLANE");
    nlohmann::json airplane;
    airplane["airplane_id"] = Input::getInput("Airplane ID (e.g. RP-C8888): ");
    airplane["model"] = Input::getInput("Model (e.g. Boeing 737): ");
    airplane["location"] = Input::getInput("Location (Airport ID): ");

    API::ApiClient& client = API::ApiClient::getInstance();
    cpr::Response apiReturn = client.post("/admin/airplane/add", airplane);
    if (apiReturn.status_code == 201 || apiReturn.status_code == 200) {
        nlohmann::json response = nlohmann::json::parse(apiReturn.text);
        std::cout << "\n  [OK] Airplane added.\n";
    } else {
        nlohmann::json errorResponse = nlohmann::json::parse(apiReturn.text);
        std::cerr << "\n  [ERROR] Failed to add airplane: "
                  << errorResponse.value("message", "Unknown error") << "\n";
    }
    std::cout << "\n  [OK] Airplane added.\n";
}

void API::Airplane::modify() {
    Display::printHeader("MODIFY AIRPLANE");
    std::string id = Input::getInput("Enter Airplane ID to modify: ");
    for (auto& a : Data::airplanes) {
        if (a.id == id) {
            std::string v;
            v = Input::getInput("New Model [" + a.model + "]: ");
            if (!v.empty()) a.model = v;
            v = Input::getInput("New Location [" + a.location + "]: ");
            if (!v.empty()) a.location = v;
            API::Airplane::save();
            std::cout << "\n  [OK] Airplane updated.\n";
            return;
        }
    }
    std::cout << "\n  [!!] Airplane not found.\n";
}

void API::Airplane::remove() {
    Display::printHeader("DELETE AIRPLANE");
    std::string id = Input::getInput("Enter Airplane ID to delete: ");
    API::ApiClient& client = API::ApiClient::getInstance();
    cpr::Response apiReturn = client.del("/admin/airplane/delete/" + id);
     std::cout << "\n  [OK] Airplane deleted.\n";
}