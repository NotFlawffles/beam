#include "../../../../../include/beam/text/parser/expression/literal_expression.hpp"

template<typename T>
T Beam::Text::Parser::Expression::LiteralExpression::getValue() const {
    switch (type) {
        case LiteralExpressionTypeName:
            return nameValue;

        case LiteralExpressionTypeInteger:
            return integerValue;

        case LiteralExpressionTypeFloat:
            return floatValue;

        case LiteralExpressionTypeCharacter:
            return characterValue;

        case LiteralExpressionTypeString:
            return stringValue;

        default:
            return "Unreachable";
    }
}

std::string
Beam::Text::Parser::Expression::LiteralExpression::getTypeAsString() const {
    switch (type) {
        case LiteralExpressionTypeName:
            return "LiteralExpressionTypeString";

        case LiteralExpressionTypeInteger:
            return "LiteralExpressionTypeInteger";

        case LiteralExpressionTypeFloat:
            return "LiteralExpressionTypeFloat";

        case LiteralExpressionTypeCharacter:
            return "LiteralExpressionTypeCharacter";

        case LiteralExpressionTypeString:
            return "LiteralExpressionTypeString";

        default:
            return "Unreachable";
    }
}
