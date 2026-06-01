#include <iostream>
#include <iomanip>
#include "./display.h"
using namespace std;

// UTILITIES
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pause() {
    cout << "\n  Press Enter to continue...";
    cin.get();
}

void printHeader(const string& title) {
    cout << "\n";
    cout << "  *------------------------------------------*\n";
    cout << "  |  " << left << setw(42) << title << "|\n";
    cout << "  *------------------------------------------*\n\n";
}

void printDivider() {
    cout << "  ------------------------------------------\n";
}

string bookingStatusToStr(Structs::BookingStatus s) {
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

Structs::BookingStatus strToBookingStatus(const string& s) {
    if (s == "confirmed") return Structs::BookingStatus::CONFIRMED;
    if (s == "cancelled") return Structs::BookingStatus::CANCELLED;
    return Structs::BookingStatus::PENDING;
}