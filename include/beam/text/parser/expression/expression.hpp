#pragma once

#include "binary.hpp"
#include "literal.hpp"
#include "unary.hpp"

namespace Beam::Text::Parser::Expression {
class Expression {
  public:
    enum Type {
        ExpressionTypeBinary,
        ExpressionTypeUnary,
        ExpressionTypeLiteral
    };

    explicit Expression(const Type& type): type(type) {}

    void addBinary(Binary* binaryExpression) {
        this->binaryExpression = binaryExpression;
    }

    void addUnary(Unary* unaryExpression) {
        this->unaryExpression = unaryExpression;
    }

    void addLiteral(Literal* literalExpression) {
        this->literalExpression = literalExpression;
    }

    template<typename T> T getExpression() const;

    std::string getTypeAsString() const;

  private:
    const Type type;

    Binary* binaryExpression;
    Unary* unaryExpression;
    Literal* literalExpression;
};
} // namespace Beam::Text::Parser::Expression
