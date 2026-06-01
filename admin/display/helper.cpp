#include <iostream>
#include <iomanip>
#include "./display.h"

using namespace Skybridge;
// UTILITIES
void Display::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Display::pause() {
    std::cout << "\n  Press Enter to continue...";
    std::cin.get();
}

void Display::printHeader(const std::string& title) {
    std::cout << "\n";
    std::cout << "  *------------------------------------------*\n";
    std::cout << "  |  " << std::left << std::setw(42) << title << "|\n";
    std::cout << "  *------------------------------------------*\n\n";
}

void Display::printDivider() {
    std::cout << "  ------------------------------------------\n";
}

std::string Display::bookingStatusToStr(Structs::BookingStatus s) {
    switch (s) {
        case Structs::BookingStatus::PENDING:
            return "pending";
        case Structs::BookingStatus::CONFIRMED:
            return "confirmed";
        case Structs::BookingStatus::CANCELLED:
            return "cancelled";
    }
    return "pending";
}

Structs::BookingStatus Display::strToBookingStatus(const std::string& s) {
    if (s == "confirmed") return Structs::BookingStatus::CONFIRMED;
    if (s == "cancelled") return Structs::BookingStatus::CANCELLED;
    return Structs::BookingStatus::PENDING;
}