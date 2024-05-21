#include "../../../../include/beam/io/string/span.hpp"

std::string Beam::IO::String::Span::format() {
    return getPath() + getStream() + '[' + std::to_string(*getRow()) + ':' +
           std::to_string(*getColumn()) + ']';
}

std::string Beam::IO::String::Span::debug() {
    return "Span(stream: \"" + getStream() + "\", path: \"" + path +
           "\", index: " + std::to_string(*getIndex()) +
           ", row: " + std::to_string(*getRow()) +
           ", column: " + std::to_string(*getColumn()) +
           ", length: " + std::to_string(*getLength()) + ")";
}
