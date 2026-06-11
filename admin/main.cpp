#include "main.h"


#include <iostream>


#include "./display/display.h"
#include "./api/api.h"

// MAIN
int main() {
    if (!Skybridge::Menu::authenticate()) return 1;
    Skybridge::Display::pause();
    Skybridge::Data::loadAll();
    Skybridge::Menu::mainMenu();
    return 0;
}

void Skybridge::Data::loadAll() {
    Skybridge::API::Account::load();
    Skybridge::API::Airport::load();
    Skybridge::API::Airplane::load();
    Skybridge::API::Booking::load();
    Skybridge::API::Passenger::load();
    Skybridge::API::Flight::load();
    Skybridge::API::SeatClass::load();
    Skybridge::API::Staff::load();
}