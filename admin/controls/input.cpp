#include <iostream>

#include "./controls.h"

using namespace Skybridge;

std::string Input::getInput(const std::string& prompt) {
    std::string val;
    do {
        std::cout << "  " << prompt;
        getline(std::cin, val);
        if (val.empty()) std::cout << "Input cannot be blank. Try again.\n";
    } while (val.empty());
    return val;
}

long long Input::getLLInput(const std::string& prompt) {
    while (true) {
        std::string value = Input::getInput(prompt);
        try { return std::stoll(value); }
        catch (...) { std::cout << "Invalid number. Try again.\n"; }
    }
}

int Input::getIntInput(const std::string& prompt) {
    while (true) {
        std::string value = Input::getInput(prompt);
        try { return std::stoi(value); }
        catch (...) { std::cout << "Invalid number. Try again.\n"; }
    }
}

double Input::getDoubleInput(const std::string& prompt) {
    while (true) {
        std::string value = Input::getInput(prompt);
        try { return std::stod(value); }
        catch (...) { std::cout << "Invalid number. Try again.\n"; }
    }
}