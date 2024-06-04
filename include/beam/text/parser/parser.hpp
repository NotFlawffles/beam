#pragma once

#include <vector>

#include "../../io/format/types/vector.hpp"
#include "../lexer/lexer.hpp"
#include "expression/expression.hpp"
#include "syntax/ast.hpp"
#include "syntax/statement/declaration/function/parameter.hpp"

namespace Beam::Text::Parser {
class Parser {
  public:
    explicit Parser(Lexer::Lexer* lexer)
        : lexer(lexer), current(lexer->lexNext()) {}

    Diagnostic::DiResult<Syntax::AbstractSyntaxTree*,
                         IO::Format::Types::Vector<Diagnostic::Diagnostic*>*>
    parse(), parseBlock(const Lexer::Token::Type& until), parseNext(),
        parseName(), parseExpression(), parseReservedWord(), parseIfStatement(),
        parseWhileStatement(), parseReturnStatement(), parseAnnotationType(),
        parseDeclaration(),
        parseVariableDeclaration(const IO::Format::Types::String& name,
                                 IO::Format::Types::Char* flags),
        parseFunctionDeclaration(const IO::Format::Types::String& name,
                                 IO::Format::Types::Char* flags),
        parseDoStatement();

    Diagnostic::DiResult<IO::Format::Types::Char*,
                         IO::Format::Types::Vector<Diagnostic::Diagnostic*>*>
    parseDeclarationFlags();

    Diagnostic::DiResult<
        IO::Format::Types::Vector<
            Syntax::Statement::Declaration::Function::Parameter*>*,
        IO::Format::Types::Vector<Diagnostic::Diagnostic*>*>
    parseFunctionParameters();

    Diagnostic::DiResult<Beam::Text::Parser::Expression::Expression*,
                         IO::Format::Types::Vector<Diagnostic::Diagnostic*>*>
    parseAssignmentExpression(), parseLogicalOrExpression(),
        parseLogicalAndExpression(), parseBitwiseOrExpression(),
        parseBitwiseXorExpression(), parseBitwiseAndExpression(),
        parseEqualityExpression(), parseRelationalExpression(),
        parseShiftExpression(), parseAdditiveExpression(),
        parseMultiplicativeExpression(), parseUnaryExpression(),
        parsePostfixExpression(),
        parseFunctionCall(Expression::Expression* expression),
        parseLiteralExpression();

    Diagnostic::DiResult<IO::Format::Types::String*,
                         IO::Format::Types::Vector<Diagnostic::Diagnostic*>*>
    getName() const;

    Diagnostic::DiResult<Lexer::Token*, Diagnostic::Diagnostic*>
    eat(const std::vector<Lexer::Token::Type>& types), skip();

    Diagnostic::DiResult<IO::Format::Types::String*, Diagnostic::Diagnostic*>
    expect(const std::vector<std::string>& names) const;

    bool currentIsReservedWord() const;

  private:
    Lexer::Lexer* lexer;
    Diagnostic::DiResult<Lexer::Token*, Diagnostic::Diagnostic*> current;
    Diagnostic::Diagnostic* currentError;
};
} // namespace Beam::Text::Parser
