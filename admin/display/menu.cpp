#include <algorithm>
#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/screen.hpp>
#include <iostream>
#include <type_traits>

#include "../api/api.h"
#include "../controls/controls.h"
#include "./display.h"

using namespace Skybridge;
using namespace API;

bool Menu::contains(std::vector<std::string>& vector, std::string value) {
    auto it = std::find(vector.begin(), vector.end(), value);
    if (it != vector.end()) {
        return true;
    } else {
        return false;
    }
}

void Menu::subMenu(auto& apiCaller) {
    int choice = -1;
    bool hasAdd =
        !Menu::contains(apiCaller.UNSUPPORTED_OPS, std::string{"add"});
    bool hasView =
        !Menu::contains(apiCaller.UNSUPPORTED_OPS, std::string{"view"});
    bool hasModify =
        !Menu::contains(apiCaller.UNSUPPORTED_OPS, std::string{"modify"});

    while (choice != 0) {
        if (!((choice == -1) || (choice == 1))) Display::pause();
        Display::clearScreen();
        Display::printHeader(apiCaller.name + " - TABLE MENU");
        if (hasView) std::cout << "  [1] View All\n";
        if (hasAdd) std::cout << "  [2] Add New\n";
        if (hasModify) std::cout << "  [3] Modify\n";
        std::cout << "  [4] Delete\n";
        std::cout << "  [0] Back to Main Menu\n\n";
        std::string ch = Input::getInput("Choice: ");
        try {
            choice = std::stoi(ch);
        } catch (const std::invalid_argument&) {
            std::cerr << "Error: Invalid input. Please enter a valid integer."
                      << std::endl;
            Display::pause();
            continue;
        } catch (const std::out_of_range&) {
            std::cerr << "Error: Input out of range." << std::endl;
            Display::pause();
            continue;
        }

        Display::clearScreen();
        if (hasView && choice == 1) {
            std::vector<std::vector<std::string>> data = apiCaller.view();
            Display::TableInteractive(data);
        } else if (hasAdd && choice == 2) {
            apiCaller.add();
        } else if (hasModify && choice == 3) {
            std::string id = Input::getInput(apiCaller.name + "ID: ");
            try {
                std::vector<std::vector<std::string>> data =
                    apiCaller.view_one(id);
                Display::Table(data);
                std::string field = Input::getInput("Field to Modify: ");
                std::string value = Input::getInput("New Value: ");
                std::vector<std::vector<std::string>> newData =
                    apiCaller.modify(id, field, value);
                Display::Table(newData);
            } catch (const std::runtime_error& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        } else if (choice == 4) {
            apiCaller.remove();
        } else if (choice == 0) {
            break;
        } else {
            std::cout << "\n  Invalid choice.\n";
        }
    }
}

void Menu::mainMenu() {
    Account& account = Account::getInstance();
    Airplane& airplane = Airplane::getInstance();
    Airport& airport = Airport::getInstance();
    Booking& booking = Booking::getInstance();
    Flight& flight = Flight::getInstance();
    Passenger& passenger = Passenger::getInstance();
    int choice = -1;
    while (choice != 0) {
        Display::clearScreen();
        std::cout << "\n";
        std::cout << "  *------------------------------------------*\n";
        std::cout << "  |      AIRLINE MANAGEMENT SYSTEM           |\n";
        std::cout << "  |           Main Menu                      |\n";
        std::cout << "  *------------------------------------------*\n\n";
        std::cout << "  Select a table to manage:\n\n";
        std::cout << "   [1]  Account\n";
        std::cout << "   [2]  Airport\n";
        // std::cout << "   [3]  Passenger\n";
        std::cout << "   [4]  Flight\n";
        std::cout << "   [5]  Airplane\n";
        std::cout << "   [6]  Booking\n";
        std::cout << "\n   [0]  Exit\n\n";

        std::string ch = Input::getInput("Choice: ");

        try {
            choice = std::stoi(ch);
        } catch (const std::invalid_argument&) {
            std::cerr << "Error: Invalid input. Please enter a valid integer."
                      << std::endl;
            Display::pause();
            continue;
        } catch (const std::out_of_range&) {
            std::cerr << "Error: Input out of range." << std::endl;
            Display::pause();
            continue;
        }

        switch (choice) {
            case 1: {
                Menu::subMenu(account);
                break;
            }
            case 2: {
                Menu::subMenu(airport);
                break;
            }
            // case 3: {
            //     Menu::subMenu(passenger);
            //     break;
            // }
            case 4: {
                Menu::subMenu(flight);
                break;
            }
            case 5: {
                Menu::subMenu(airplane);
                break;
            }
            case 6: {
                Menu::subMenu(booking);
                break;
            }
            case 0: {
                std::cout << "\n  Goodbye!\n\n";
                break;
            }
            default: {
                Display::clearScreen();
                std::cout << "\n  Invalid choice.\n";
                Display::pause();
                break;
            }
        }
    }
}

bool Menu::authenticate() {
    auto& auth = API::Auth::getInstance();
    Display::clearScreen();
    std::cout << "\n";
    std::cout << "  *------------------------------------------*\n";
    std::cout << "  |      AIRLINE MANAGEMENT SYSTEM           |\n";
    std::cout << "  |           Access Required                |\n";
    std::cout << "  *------------------------------------------*\n\n";
    int attempts = 3;
    while (attempts-- > 0) {
        std::string email = Input::getInput("Email: ");
        std::string pass = Input::getInput("Password: ");
        try {
            bool response = auth.login(email, pass);
            if (response) {
                std::cout << "\n  [OK] Access granted.\n";
                return true;
            }
            std::cout << "  [!!] Incorrect. " << attempts
                      << " attempt(s) remaining.\n\n";
        } catch (const std::runtime_error& e) {
            std::cout << "\n  [!!] Server error: " << e.what() << "\n";
            std::cout << "  Unable to reach server. Exiting.\n\n";
            return false;
        }
    }
    std::cout << "\n  Access denied. Exiting.\n\n";
    return false;
}