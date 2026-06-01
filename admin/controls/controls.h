#pragma once
#include <string>
#include <vector>

#include "../main.h"
using namespace Skybridge;

namespace Escape {
std::string escape(const std::string& s);
std::string unescape(const std::string& s);
std::vector<std::string> splitLine(const std::string& line);
}  // namespace Escape

namespace Input {
std::string getInput(const std::string& prompt);
long long getLLInput(const std::string& prompt);
int getIntInput(const std::string& prompt);
double getDoubleInput(const std::string& prompt);
}  // namespace Input