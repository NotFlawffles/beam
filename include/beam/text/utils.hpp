#pragma once

#include <string>
#include <vector>

namespace Beam::Text::Utils {
std::vector<std::string> splitString(const std::string& string,
                                     const char& delimeter);
}