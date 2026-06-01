#include <iostream>
#include <vector>
#include <sstream>
#include "./controls.h"
using namespace std;

// Escape/unescape pipe characters in stored strings
string escape(const string& s) {
    string r = s;
    size_t pos = 0;
    while ((pos = r.find('|', pos)) != string::npos) {
        r.replace(pos, 1, "[PIPE]");
        pos += 6;
    }
    return r;
}

string unescape(const string& s) {
    string r = s;
    size_t pos = 0;
    while ((pos = r.find("[PIPE]", pos)) != string::npos) {
        r.replace(pos, 6, "|");
        pos += 1;
    }
    return r;
}

vector<string> splitLine(const string& line) {
    vector<string> tokens;
    stringstream ss(line);
    string token;
    while (getline(ss, token, '|')) tokens.push_back(unescape(token));
    return tokens;
}