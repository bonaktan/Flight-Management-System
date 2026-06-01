#include <iostream>
#include <vector>
#include <sstream>
#include "./controls.h"
using namespace Skybridge;
// Escape/unescape pipe characters in stored strings
std::string Escape::escape(const std::string& s) {
    std::string r = s;
    size_t pos = 0;
    while ((pos = r.find('|', pos)) != std::string::npos) {
        r.replace(pos, 1, "[PIPE]");
        pos += 6;
    }
    return r;
}

std::string Escape::unescape(const std::string& s) {
    std::string r = s;
    size_t pos = 0;
    while ((pos = r.find("[PIPE]", pos)) != std::string::npos) {
        r.replace(pos, 6, "|");
        pos += 1;
    }
    return r;
}

std::vector<std::string> Escape::splitLine(const std::string& line) {
    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string token;
    while (getline(ss, token, '|')) tokens.push_back(Escape::unescape(token));
    return tokens;
}