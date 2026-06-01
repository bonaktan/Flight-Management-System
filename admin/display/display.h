#pragma once
#include <string>

#include "../main.h"
using namespace Skybridge;

namespace Display {
void clearScreen();
void pause();
void printHeader(const std::string& title);
void printDivider();
std::string bookingStatusToStr(Structs::BookingStatus s);
Structs::BookingStatus strToBookingStatus(const std::string& s);
}  // namespace Display