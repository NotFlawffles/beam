#include "../../../../../../../../include/beam/text/parser/syntax/annotation/declaration/type/name.hpp"

std::string
Beam::Text::Parser::Syntax::Annotation::Declaration::Type::Name::format() {
    return getValue()->format();
}

std::string
Beam::Text::Parser::Syntax::Annotation::Declaration::Type::Name::debug() {
    return "Name(" + format() + ')';
}
