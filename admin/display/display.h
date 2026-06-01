#pragma once

#ifndef SKYBRIDGE_DISPLAY
#define SKYBRIDGE_DISPLAY

#include <string>

#include "../main.h"


namespace Skybridge::Display {
void clearScreen();
void pause();
void printHeader(const std::string& title);
void printDivider();
std::string bookingStatusToStr(Structs::BookingStatus s);
Structs::BookingStatus strToBookingStatus(const std::string& s);
}  // namespace Display

namespace Skybridge::Menu {
void subMenu(const std::string& title, void (*viewFn)(), void (*addFn)(),
             void (*modFn)(), void (*delFn)());
void mainMenu();
bool authenticate();
}  // namespace Menu

#endif