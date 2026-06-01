#include <iostream>

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
        return -1;
    }
}

int Input::getIntInput(const std::string& prompt) {
    std::string line = Input::getInput(prompt);
    try {
        return stoi(line);
    } catch (...) {
        return 0;
    }
}

double Input::getDoubleInput(const std::string& prompt) {
    std::string line = Input::getInput(prompt);
    try {
        return stod(line);
    } catch (...) {
        return 0.0;
    }
}