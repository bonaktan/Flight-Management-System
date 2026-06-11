#pragma once

#include <any>
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
void subMenu(auto& apiCaller);
void mainMenu();
bool authenticate();
bool contains(std::vector<std::string>& vector, std::string value);
}  // namespace Menu

#endif