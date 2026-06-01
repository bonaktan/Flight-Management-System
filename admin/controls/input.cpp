#include <iostream>

#include "./controls.h"

using namespace std;

string getInput(const string& prompt) {
    string val;
    cout << "  " << prompt;
    getline(cin, val);
    return val;
}

long long getLLInput(const string& prompt) {
    string line = getInput(prompt);
    try {
        return stoll(line);
    } catch (...) {
        return -1;
    }
}

int getIntInput(const string& prompt) {
    string line = getInput(prompt);
    try {
        return stoi(line);
    } catch (...) {
        return 0;
    }
}

double getDoubleInput(const string& prompt) {
    string line = getInput(prompt);
    try {
        return stod(line);
    } catch (...) {
        return 0.0;
    }
}