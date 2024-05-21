#include "../../../../include/beam/io/string/utils.hpp"

std::vector<std::string>
Beam::IO::String::Utils::splitString(const std::string& str,
                                     const char& delimiter) {
    std::vector<std::string> splits;

    std::size_t position = 0, previous = 0;

    while ((position = str.find(delimiter, previous)) != std::string::npos) {
        splits.push_back(str.substr(previous, position - previous));
        previous = position + 1;
    }

    splits.push_back(str.substr(previous));
    return splits;
}
