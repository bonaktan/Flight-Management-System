#include "main.h"


#include <iostream>


#include "./display/display.h"
#include "./api/api.h"

// MAIN
int main() {
    if (!Skybridge::Menu::authenticate()) return 1;
    Skybridge::Display::pause();
    Skybridge::Menu::mainMenu();
    return 0;
}