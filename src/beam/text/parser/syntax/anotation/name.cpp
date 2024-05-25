#include "../../../../../../include/beam/text/parser/syntax/anotation/type/name.hpp"

std::string Beam::Text::Parser::Syntax::Anotation::Type::Name::format() {
    return getValue()->format();
}

std::string Beam::Text::Parser::Syntax::Anotation::Type::Name::debug() {
    return "Name(" + format() + ')';
}
