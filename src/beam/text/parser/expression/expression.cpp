#include "../../../../../include/beam/text/parser/expression/expression.hpp"

template<typename T>
T Beam::Text::Parser::Expression::Expression::getExpression() const {
    switch (type) {
        case ExpressionTypeBinary:
            return binaryExpression;

        case ExpressionTypeUnary:
            return unaryExpression;

        case ExpressionTypeLiteral:
            return literalExpression;

        default:
            return "Unreachable";
    }
}

std::string
Beam::Text::Parser::Expression::Expression::getTypeAsString() const {
    switch (type) {
        case ExpressionTypeBinary:
            return "ExpressionTypeBinary";

        case ExpressionTypeUnary:
            return "ExpressionTypeUnary";

        case ExpressionTypeLiteral:
            return "ExpressionTypeLiteral";

        default:
            return "Unreachable";
    }
}
