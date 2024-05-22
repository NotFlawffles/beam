#include "../../../../../include/beam/text/parser/expression/expression.hpp"

std::string
Beam::Text::Parser::Expression::Expression::getTypeAsString() const {
    switch (type) {
        case Type::Binary:
            return "Binary";

        case Type::Unary:
            return "Unary";

        case Type::Literal:
            return "Literal";

        default:
            return "Unreachable";
    }
}
