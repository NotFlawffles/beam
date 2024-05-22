#include "../../../../../include/beam/text/parser/expression/literal.hpp"
#include <cstdio>

std::string
Beam::Text::Parser::Expression::Literal::getLiteralTypeAsString() const {
    switch (literalType) {
        case Type::Name:
            return "Name";

        case Type::Integer:
            return "Integer";

        case Type::Float:
            return "Float";

        case Type::Character:
            return "Character";

        case Type::String:
            return "String";

        default:
            return "Unreachable";
    }
}

std::string Beam::Text::Parser::Expression::Literal::format() {
    return debug();
}

std::string Beam::Text::Parser::Expression::Literal::debug() {
    auto content =
        std::string("Literal(type: " + getLiteralTypeAsString() + ", value: ");

    switch (literalType) {
        case Type::Name:
            content.append(nameValue);
            break;

        case Type::Integer:
            content.append(std::to_string(integerValue));
            break;

        case Type::Float:
            content.append(std::to_string(floatValue));
            break;

        case Type::Character:
            content.append({'\'', characterValue, '\''});
            break;

        case Type::String:
            content.append('\"' + stringValue + '\"');
            break;

        default:
            content.append("Unreachable");
            break;
    }

    content.push_back(')');
    return content;
}
