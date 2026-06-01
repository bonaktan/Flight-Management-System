#include <iostream>
#include "./display.h"
#include "../controls/controls.h"
using namespace Skybridge;
void subMenu(const std::string& title, void (*viewFn)(), void (*addFn)(),
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

// MAIN MENU
void mainMenu() {
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
        std::cout << "   [9]  Flight Staff  (junction)\n";
        std::cout << "  [10]  Airport Flight (junction)\n";
        std::cout << "\n   [0]  Exit\n\n";
        std::string ch = Input::getInput("Choice: ");
        if (ch == "1")
            subMenu("ACCOUNT", viewAccounts, addAccount, modifyAccount,
                    deleteAccount);
        else if (ch == "2")
            subMenu("AIRPORT", viewAirports, addAirport, modifyAirport,
                    deleteAirport);
        else if (ch == "3")
            subMenu("STAFF", viewStaffs, addStaff, modifyStaff, deleteStaff);
        else if (ch == "4")
            subMenu("PASSENGER", viewPassengers, addPassenger, modifyPassenger,
                    deletePassenger);
        else if (ch == "5")
            subMenu("FLIGHT", viewFlights, addFlight, modifyFlight,
                    deleteFlight);
        else if (ch == "6")
           subMenu("AIRPLANE", viewAirplanes, addAirplane, modifyAirplane,
                    deleteAirplane);
        else if (ch == "7")
            subMenu("SEAT CLASS", viewSeatClasses, addSeatClass,
                    modifySeatClass, deleteSeatClass);
        else if (ch == "8")
            subMenu("BOOKING", viewBookings, addBooking, modifyBooking,
                    deleteBooking);
        else if (ch == "9")
            subMenu("FLIGHT STAFF", viewFlightStaffs, addFlightStaff,
                    modifyFlightStaff, deleteFlightStaff);
        else if (ch == "10")
            subMenu("AIRPORT FLIGHT", viewAirportFlights, addAirportFlight,
                    modifyAirportFlight, deleteAirportFlight);
        else if (ch == "0") {
            std::cout << "\n  Goodbye!\n\n";
            break;
        } else {
            clearScreen();
            std::cout << "\n  Invalid choice.\n";
            pause();
        }
    }
}

// PASSWORD
bool authenticate() {
    clearScreen();
    std::cout << "\n";
    std::cout << "  *------------------------------------------*\n";
    std::cout << "  |      AIRLINE MANAGEMENT SYSTEM           |\n";
    std::cout << "  |           Access Required                |\n";
    std::cout << "  *------------------------------------------*\n\n";
    // Password here
    const string CORRECT_PASSWORD = "12345678";
    // For Attempt
    int attempts = 3;
    while (attempts-- > 0) {
        string pass = getInput("Password: ");
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