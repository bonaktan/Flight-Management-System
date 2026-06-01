#include <iostream>

#include "../display/display.h"
#include "./controls.h"
using namespace Skybridge;

std::string Input::getInput(const std::string& prompt) {
    std::string val;
    std::cout << "  " << prompt;
    getline(std::cin, val);
    return val;
}

long long Input::getLLInput(const std::string& prompt) {
    std::string line = getInput(prompt);
    try {
        return stoll(line);
    } catch (...) {
        std::cerr << "ERROR: Invalid Input entered." << std::endl;
        Display::pause();
        return -1;
    }
}

int Input::getIntInput(const std::string& prompt) {
    std::string line = Input::getInput(prompt);
    try {
        return stoi(line);
    } catch (...) {
        std::cerr << "ERROR: Invalid Input entered." << std::endl;
        Display::pause();
        return -1;
    }
}

double Input::getDoubleInput(const std::string& prompt) {
    std::string line = Input::getInput(prompt);
    try {
        return stod(line);
    } catch (...) {
        std::cerr << "ERROR: Invalid Input entered." << std::endl;
        Display::pause();
        return 0.0;
    }
}
