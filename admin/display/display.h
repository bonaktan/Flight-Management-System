#pragma once

#ifndef SKYBRIDGE_DISPLAY
#define SKYBRIDGE_DISPLAY
#include <any>
#include <ftxui/component/screen_interactive.hpp>
#include <string>

#include "../main.h"

namespace Skybridge::Display {
void clearScreen();
void pause();
void printHeader(const std::string& title);
void printDivider();
std::string bookingStatusToStr(Structs::BookingStatus s);
Structs::BookingStatus strToBookingStatus(const std::string& s);
void Table(std::vector<std::vector<std::string>>& data);
ftxui::Component TableInteractiveComponent(
    std::vector<std::vector<std::string>>& data, std::function<void()> on_quit);
}  // namespace Skybridge::Display

namespace Skybridge::Menu {
void mainMenu();
bool authenticate();
bool contains(std::vector<std::string>& vector, std::string value);
}  // namespace Skybridge::Menu

#endif