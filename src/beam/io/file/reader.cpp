#include "../../../../include/beam/io/file/reader.hpp"
#include "../../../../include/beam/io/string/span.hpp"
#include "../../../../include/beam/io/string/utils.hpp"

#include <fstream>
#include <iostream>

std::string Beam::IO::File::Reader::readAll() const { return content; }

std::string
Beam::IO::File::Reader::readLine(const unsigned long long int& index) {
    return String::Utils::splitString(content, '\n').at(index);
}

std::string Beam::IO::File::Reader::format() {
    auto content = std::string("-- " + path + " --\n");

    for (std::size_t index = 0;
         index < String::Utils::splitString(readAll(), '\n').size(); index++) {
        content.append(std::to_string(index) + '\t' + readLine(index) + '\n');
    }

    return content;
}

std::string Beam::IO::File::Reader::debug() {
    return "Reader(path: \"" + path + "\", content: [content; " +
           std::to_string(readAll().length()) + " bytes]}";
}
