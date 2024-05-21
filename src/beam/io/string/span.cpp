#include "../../../../include/beam/io/string/span.hpp"

std::string Beam::IO::String::Span::debug() {
    return "Span(stream: \"" + getStream() + "\", " +
           "index: " + std::to_string(*getIndex()) +
           ", row: " + std::to_string(*getRow()) +
           ", column: " + std::to_string(*getColumn()) +
           ", length: " + std::to_string(*getLength()) + ")";
}
