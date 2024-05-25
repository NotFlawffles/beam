#include "../../../../../../include/beam/text/parser/syntax/anotation/type/pointer.hpp"

std::string Beam::Text::Parser::Syntax::Anotation::Type::Pointer::format() {
    return "*";
}

std::string Beam::Text::Parser::Syntax::Anotation::Type::Pointer::debug() {
    return "Pointer(...)";
}
