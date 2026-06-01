#include "main.h"

#include "./display/display.h"
#include "api/api.h"
// MAIN
int main() {
    if (!Skybridge::Menu::authenticate()) return 1;
    Skybridge::Display::pause();
    Skybridge::Data::loadAll();
    Skybridge::Menu::mainMenu();
    return 0;
}

void Skybridge::Data::loadAll() {
    API::Account::load();
    API::Airport::load();
    API::Airplane::load();
    API::Booking::load();
    API::Passenger::load();
    API::Flight::load();
    API::SeatClass::load();
    API::Staff::load();
}