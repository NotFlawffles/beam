#pragma once

#include <vector>

#include "../../io/format/types/vector.hpp"
#include "../lexer/lexer.hpp"
#include "expression/expression.hpp"
#include "syntax/ast.hpp"

namespace Beam::Text::Parser {
class Parser {
  public:
    explicit Parser(Lexer::Lexer* lexer)
        : lexer(lexer), current(lexer->lexNext()) {}

    Diagnostic::Result<Syntax::AbstractSyntaxTree*,
                       IO::Format::Types::Vector<Diagnostic::Error*>*>
    parse(), parseBlock(const Lexer::Token::Type& until), parseNext(),
        parseName(), parseExpression(), parseReservedWord(),
        parseConditionalStatement();

    Diagnostic::Result<Beam::Text::Parser::Expression::Expression*,
                       IO::Format::Types::Vector<Diagnostic::Error*>*>
    parseListExpression(), parseAssignmentExpression(),
        parseLogicalOrExpression(), parseLogicalAndExpression(),
        parseBitwiseOrExpression(), parseBitwiseXorExpression(),
        parseBitwiseAndExpression(), parseEqualityExpression(),
        parseRelationalExpression(), parseShiftExpression(),
        parseAdditiveExpression(), parseMultiplicativeExpression(),
        parseUnaryExpression(), parsePostfixExpression(),
        parseLiteralExpression();

    Diagnostic::Result<Lexer::Token*, Diagnostic::Error*>
    eat(const std::vector<Lexer::Token::Type>& types);

    bool currentIsReservedWord() const;

  private:
    Lexer::Lexer* lexer;
    Diagnostic::Result<Lexer::Token*, Diagnostic::Error*> current;
};
} // namespace Beam::Text::Parser
