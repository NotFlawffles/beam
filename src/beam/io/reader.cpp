#include "../../../include/beam/io/reader.hpp"
#include "../../../include/beam/text/span.hpp"
#include "../../../include/beam/text/utils.hpp"

#include <fstream>
#include <iostream>

std::string Beam::IO::Reader::readAll() const { return content; }

std::string Beam::IO::Reader::readLine(const unsigned long long int& index) {
    return Text::Utils::splitString(content, '\n').at(index);
}

std::string Beam::IO::Reader::debug() {
    return "Reader(path: \"" + path + "\", content: [content; " +
           std::to_string(readAll().length()) + " bytes]}";
}
