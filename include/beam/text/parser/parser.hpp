#pragma once

#include <vector>

#include "../../io/format/types/vector.hpp"
#include "../lexer/lexer.hpp"
#include "expression/expression.hpp"
#include "syntax/ast.hpp"

namespace Beam::Text::Parser {
class Parser {
  public:
    explicit Parser(Lexer::Lexer* lexer): lexer(lexer) {}

    Diagnostic::DiResult<Syntax::AbstractSyntaxTree*,
                         IO::Format::Types::Vector<Diagnostic::Error*>*>
    parse(), parseBlock(const Lexer::Token::Type& until), parseNext(),
        parseName(), parseExpression(), parseReservedWord(), parseIfStatement(),
        parseWhileStatement(), parseReturnStatement(), parseAnotationType(),
        parseDeclaration();

    Diagnostic::DiResult<Beam::Text::Parser::Expression::Expression*,
                         IO::Format::Types::Vector<Diagnostic::Error*>*>
    parseListExpression(), parseAssignmentExpression(),
        parseLogicalOrExpression(), parseLogicalAndExpression(),
        parseBitwiseOrExpression(), parseBitwiseXorExpression(),
        parseBitwiseAndExpression(), parseEqualityExpression(),
        parseRelationalExpression(), parseShiftExpression(),
        parseAdditiveExpression(), parseMultiplicativeExpression(),
        parseUnaryExpression(), parsePostfixExpression(),
        parseLiteralExpression();

    Diagnostic::DiResult<Lexer::Token*, Diagnostic::Error*>
    eat(const std::vector<Lexer::Token::Type>& types), skip();

    bool currentIsReservedWord() const;

  private:
    Lexer::Lexer* lexer;
    Diagnostic::DiResult<Lexer::Token*, Diagnostic::Error*> current;
    Diagnostic::Error* currentError;
};
} // namespace Beam::Text::Parser
