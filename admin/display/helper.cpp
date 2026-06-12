#include <ftxui/dom/elements.hpp>
#include <ftxui/dom/table.hpp>
#include <ftxui/screen/screen.hpp>
#include <iomanip>
#include <iostream>

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

void Display::Table(std::vector<std::vector<std::string>>& data) {
    auto table = ftxui::Table(data);

    table.SelectAll().Border(ftxui::LIGHT);
    table.SelectAll().Separator(ftxui::LIGHT);

    table.SelectRow(0).Decorate(ftxui::bold);
    table.SelectRow(0).SeparatorVertical(ftxui::LIGHT);
    table.SelectRow(0).Border(ftxui::LIGHT);

    auto document = table.Render();

    auto screen = ftxui::Screen::Create(ftxui::Dimension::Full(),
                                        ftxui::Dimension::Fit(document));
    Render(screen, document);

    std::cout << screen.ToString() << std::endl;
}