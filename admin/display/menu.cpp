#include <iostream>

#include "../api/api.h"
#include "../controls/controls.h"
#include "./display.h"

using namespace Skybridge;
using namespace API;

void Menu::subMenu(const std::string& title, void (*viewFn)(), void (*addFn)(),
                   void (*modFn)(), void (*delFn)()) {
    while (true) {
        Display::clearScreen();
        Display::printHeader(title + " - TABLE MENU");
        std::cout << "  [1] View All\n";
        std::cout << "  [2] Add New\n";
        std::cout << "  [3] Modify\n";
        std::cout << "  [4] Delete\n";
        std::cout << "  [0] Back to Main Menu\n\n";
        std::string ch = Input::getInput("Choice: ");
        Display::clearScreen();
        if (ch == "1") {
            viewFn();
            Display::pause();
        } else if (ch == "2") {
            addFn();
            Display::pause();
        } else if (ch == "3") {
            modFn();
            Display::pause();
        } else if (ch == "4") {
            delFn();
            Display::pause();
        } else if (ch == "0")
            break;
        else {
            std::cout << "\n  Invalid choice.\n";
            Display::pause();
        }
    }
}

void Menu::mainMenu() {
    while (true) {
        Display::clearScreen();
        std::cout << "\n";
        std::cout << "  *------------------------------------------*\n";
        std::cout << "  |      AIRLINE MANAGEMENT SYSTEM           |\n";
        std::cout << "  |           Main Menu                      |\n";
        std::cout << "  *------------------------------------------*\n\n";
        std::cout << "  Select a table to manage:\n\n";
        std::cout << "   [1]  Account\n";
        std::cout << "   [2]  Airport\n";
        std::cout << "   [3]  Staff\n";
        std::cout << "   [4]  Passenger\n";
        std::cout << "   [5]  Flight\n";
        std::cout << "   [6]  Airplane\n";
        std::cout << "   [7]  Seat Class\n";
        std::cout << "   [8]  Booking\n";
        std::cout << "\n   [0]  Exit\n\n";
        std::string ch = Input::getInput("Choice: ");
        if (ch == "1")
            subMenu("ACCOUNT", Account::view, Account::add, Account::modify,
                    Account::remove);
        else if (ch == "2")
            subMenu("AIRPORT", Airport::view, Airport::add, Airport::modify,
                    Airport::remove);
        else if (ch == "3")
            subMenu("STAFF", Staff::view, Staff::add, Staff::modify,
                    Staff::remove);
        else if (ch == "4")
            subMenu("PASSENGER", Passenger::view, Passenger::add,
                    Passenger::modify, Passenger::remove);
        else if (ch == "5")
            subMenu("FLIGHT", Flight::view, Flight::add, Flight::modify,
                    Flight::remove);
        else if (ch == "6")
            subMenu("AIRPLANE", Airplane::view, Airplane::add, Airplane::modify,
                    Airplane::remove);
        else if (ch == "7")
            subMenu("SEAT CLASS", SeatClass::view, SeatClass::add,
                    SeatClass::modify, SeatClass::remove);
        else if (ch == "8")
            subMenu("BOOKING", Booking::view, Booking::add, Booking::modify,
                    Booking::remove);
        else if (ch == "0") {
            std::cout << "\n  Goodbye!\n\n";
            break;
        } else {
            Display::clearScreen();
            std::cout << "\n  Invalid choice.\n";
            Display::pause();
        }
    }
}

bool Menu::authenticate() {
    Display::clearScreen();
    std::cout << "\n";
    std::cout << "  *------------------------------------------*\n";
    std::cout << "  |      AIRLINE MANAGEMENT SYSTEM           |\n";
    std::cout << "  |           Access Required                |\n";
    std::cout << "  *------------------------------------------*\n\n";
    const std::string CORRECT_PASSWORD = "12345678";
    int attempts = 3;
    while (attempts-- > 0) {
        std::string pass = Input::getInput("Password: ");
        if (pass == CORRECT_PASSWORD) {
            std::cout << "\n  [OK] Access granted.\n";
            return true;
        }
        std::cout << "  [!!] Incorrect. " << attempts
                  << " attempt(s) remaining.\n\n";
    }
    std::cout << "\n  Access denied. Exiting.\n\n";
    return false;
}