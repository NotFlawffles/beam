#pragma once

#include "binary_expression.hpp"
#include "literal_expression.hpp"
#include "unary_expression.hpp"

namespace Beam::Text::Parser::Expression {
class Expression {
  public:
    enum Type {
        ExpressionTypeBinary,
        ExpressionTypeUnary,
        ExpressionTypeLiteral
    };

    explicit Expression(const Type& type): type(type) {}

    void addBinaryExpression(BinaryExpression* binaryExpression) {
        this->binaryExpression = binaryExpression;
    }

    void addUnaryExpression(UnaryExpression* unaryExpression) {
        this->unaryExpression = unaryExpression;
    }

    void addLiteralExpression(LiteralExpression* literalExpression) {
        this->literalExpression = literalExpression;
    }

    template<typename T> T getExpression() const;

    std::string getTypeAsString() const;

  private:
    const Type type;

    BinaryExpression* binaryExpression;
    UnaryExpression* unaryExpression;
    LiteralExpression* literalExpression;
};
} // namespace Beam::Text::Parser::Expression
