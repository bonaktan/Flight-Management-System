
#include "main.h"
#include "./display/display.h"

int main() {
    if (!Skybridge::Menu::authenticate()) return 1;
    Skybridge::Display::pause();
    Skybridge::Menu::mainMenu();
    return 0;
}