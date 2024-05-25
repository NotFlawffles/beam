#include <algorithm>
#include <array>
#include <cstring>
#include <memory>

#include "../../../../include/beam/compiler/utils/typechecking.hpp"
#include "../../../../include/beam/text/parser/expression/binary.hpp"
#include "../../../../include/beam/text/parser/expression/literal.hpp"
#include "../../../../include/beam/text/parser/expression/operator.hpp"
#include "../../../../include/beam/text/parser/expression/unary.hpp"
#include "../../../../include/beam/text/parser/parser.hpp"
#include "../../../../include/beam/text/parser/syntax/anotation/type/name.hpp"
#include "../../../../include/beam/text/parser/syntax/anotation/type/pointer.hpp"
#include "../../../../include/beam/text/parser/syntax/primary/block.hpp"
#include "../../../../include/beam/text/parser/syntax/primary/expression.hpp"
#include "../../../../include/beam/text/parser/syntax/statement/conditional/if.hpp"
#include "../../../../include/beam/text/parser/syntax/statement/conditional/while.hpp"
#include "../../../../include/beam/text/parser/syntax/statement/declaration/variable.hpp"
#include "../../../../include/beam/text/parser/syntax/statement/return.hpp"

#define EatAndReturnIfError(types)                                             \
    if (eat(types).isFailure())                                                \
    return new Vec(Beam::Diagnostic::Error*, {currentError})

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Syntax::AbstractSyntaxTree*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Error*>*>
Beam::Text::Parser::Parser::parse() {
    return parseBlock(Lexer::Token::Type::EndOfFile);
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Syntax::AbstractSyntaxTree*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Error*>*>
Beam::Text::Parser::Parser::parseNext() {
    if (current.isFailure()) {
        return new Vec(Diagnostic::Error*, {current.getError()});
    }

    switch (current.getValue()->getType()) {
        case Lexer::Token::Type::Identifier:
            return parseName();

        case Lexer::Token::Type::Integer:
        case Lexer::Token::Type::Float:
        case Lexer::Token::Type::Character:
        case Lexer::Token::Type::String:
        case Lexer::Token::Type::Plus:
        case Lexer::Token::Type::Minus:
        case Lexer::Token::Type::Exclamation:
        case Lexer::Token::Type::Tilde:
        case Lexer::Token::Type::LeftParenthesis:
            return parseExpression();

        case Lexer::Token::Type::LeftCurlyBrace:
            return parseBlock(Lexer::Token::Type::RightCurlyBrace);

        default:
            EatAndReturnIfError({current.getValue()->getType()});

            return new Vec(
                Diagnostic::Error*,
                {new Diagnostic::Error(
                    Diagnostic::Error::Type::ErrorTypeUnexpectedToken,
                    Diagnostic::Error::Icon::ErrorIconCurlyBracesCross,
                    lexer->span, "expected statement.")});
    }
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Syntax::AbstractSyntaxTree*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Error*>*>
Beam::Text::Parser::Parser::parseBlock(const Lexer::Token::Type& until) {
    auto tree = IO::Format::Types::Vector<Syntax::AbstractSyntaxTree*>({});
    auto errors = new IO::Format::Types::Vector<Diagnostic::Error*>({});

    if (until != Lexer::Token::Type::EndOfFile) {
        EatAndReturnIfError({Lexer::Token::Type::LeftCurlyBrace});
    } else {
        skip();
    }

    while (true) {
        if (current.isSuccess() && current.getValue()->getType() == until) {
            break;
        }

        bool currentTokenIsFailure = current.isFailure();

        auto next = parseNext();

        if (next.isSuccess()) {
            if (!Compiler::Utils::isDynamicallyCastableTo<
                    Syntax::AbstractSyntaxTree*, Syntax::Primary::Block*>(
                    next.getValue()) &&
                !Compiler::Utils::isDynamicallyCastableTo<
                    Syntax::AbstractSyntaxTree*,
                    Syntax::Statement::Conditional::Conditional*>(
                    next.getValue())) {
                auto semiColonCheck = eat({Lexer::Token::Type::SemiColon});

                if (semiColonCheck.isFailure()) {
                    errors->push_back(semiColonCheck.getError());
                }
            }

            tree.push_back(next.getValue());
        } else {
            errors->insert(errors->end(), next.getError()->begin(),
                           next.getError()->end());

            if (currentTokenIsFailure) {
                skip();
            }
        }
    }

    EatAndReturnIfError({until});

    if (errors->empty()) {
        return new Syntax::Primary::Block(tree);
    } else {
        return errors;
    }
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Syntax::AbstractSyntaxTree*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Error*>*>
Beam::Text::Parser::Parser::parseName() {
    if (currentIsReservedWord()) {
        return parseReservedWord();
    }

    return parseExpression();
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Syntax::AbstractSyntaxTree*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Error*>*>
Beam::Text::Parser::Parser::parseReservedWord() {
    if (current.getValue()->getValue() == "if") {
        return parseIfStatement();
    } else if (current.getValue()->getValue() == "else") {
        return new Vec(Diagnostic::Error*,
                       {new Diagnostic::Error(
                           Diagnostic::Error::Type::ErrorTypeUnexpectedToken,
                           Diagnostic::Error::Icon::ErrorIconCurlyBracesCross,
                           lexer->span, "expected statement.")});
    } else if (current.getValue()->getValue() == "while") {
        return parseWhileStatement();
    } else if (current.getValue()->getValue() == "return") {
        return parseReturnStatement();
    } else if (current.getValue()->getValue() == "let") {
        return parseDeclaration();
    }
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Syntax::AbstractSyntaxTree*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Error*>*>
Beam::Text::Parser::Parser::parseIfStatement() {
    auto value = current.getValue()->getValue();
    auto confirmedElse = value == "else";
    EatAndReturnIfError({Lexer::Token::Type::Identifier});

    Syntax::Primary::Expression* condition = nullptr;

    if (value == "if" ||
        (value == "else" && current.getValue()->getValue() == "if")) {
        if (current.getValue()->getValue() == "if") {
            confirmedElse = false;
            EatAndReturnIfError({Lexer::Token::Type::Identifier});
        }

        condition = dynamic_cast<Syntax::Primary::Expression*>(
            parseExpression().getValue());
    }

    DefineAndReturnIfError(block,
                           parseBlock(Lexer::Token::Type::RightCurlyBrace));

    Syntax::Statement::Conditional::If* elseClause = nullptr;

    if (!confirmedElse &&
        (currentIsReservedWord() && current.getValue()->getValue() == "else")) {
        DefineAndReturnIfError(result, parseIfStatement());

        elseClause = dynamic_cast<Syntax::Statement::Conditional::If*>(
            result.getValue());
    }

    return new Syntax::Statement::Conditional::If(
        condition, dynamic_cast<Syntax::Primary::Block*>(block.getValue()),
        elseClause);
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Syntax::AbstractSyntaxTree*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Error*>*>
Beam::Text::Parser::Parser::parseWhileStatement() {
    EatAndReturnIfError({Lexer::Token::Type::Identifier});
    DefineAndReturnIfError(condition, parseExpression());

    DefineAndReturnIfError(block,
                           parseBlock(Lexer::Token::Type::RightCurlyBrace));

    return new Syntax::Statement::Conditional::While(
        dynamic_cast<Syntax::Primary::Expression*>(condition.getValue()),
        dynamic_cast<Syntax::Primary::Block*>(block.getValue()));
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Syntax::AbstractSyntaxTree*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Error*>*>
Beam::Text::Parser::Parser::parseReturnStatement() {
    EatAndReturnIfError({Lexer::Token::Type::Identifier});
    DefineAndReturnIfError(expression, parseExpression());

    return new Syntax::Statement::Return(
        dynamic_cast<Syntax::Primary::Expression*>(expression.getValue()));
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Syntax::AbstractSyntaxTree*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Error*>*>
Beam::Text::Parser::Parser::parseAnotationType() {
    auto name = current.getValue()->getValue();

    auto type = new Syntax::Anotation::Type::Type(
        Syntax::Anotation::Type::Type::Kind::Name);

    type =
        new Syntax::Anotation::Type::Name(new IO::Format::Types::String(name));

    EatAndReturnIfError({Lexer::Token::Type::Identifier});

    while (current.isSuccess() &&
           current.getValue()->getType() == Lexer::Token::Type::Asterisk) {
        type = new Syntax::Anotation::Type::Pointer(type);
        EatAndReturnIfError({Lexer::Token::Type::Asterisk});
    }

    return type;
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Syntax::AbstractSyntaxTree*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Error*>*>
Beam::Text::Parser::Parser::parseDeclaration() {
    EatAndReturnIfError({Lexer::Token::Type::Identifier});

    if (current.isFailure()) {
        return new Vec(Diagnostic::Error*, {current.getError()});
    }

    auto name = current.getValue()->getValue();
    EatAndReturnIfError({Lexer::Token::Type::Identifier});

    // implement functions

    EatAndReturnIfError({Lexer::Token::Type::Colon});
    DefineAndReturnIfError(anotationType, parseAnotationType());
    EatAndReturnIfError({Lexer::Token::Type::Assign});
    DefineAndReturnIfError(value, parseExpression());

    return new Syntax::Statement::Declaration::Variable(
        dynamic_cast<Syntax::Anotation::Type::Type*>(anotationType.getValue()),
        name, dynamic_cast<Syntax::Primary::Expression*>(value.getValue()));
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Syntax::AbstractSyntaxTree*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Error*>*>
Beam::Text::Parser::Parser::parseExpression() {
    DefineAndReturnIfError(expression, parseListExpression());
    return new Syntax::Primary::Expression(expression.getValue());
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Expression::Expression*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Error*>*>
Beam::Text::Parser::Parser::parseListExpression() {
    DefineAndReturnIfError(left, parseAssignmentExpression());
    auto targets = std::vector<Lexer::Token::Type>({Lexer::Token::Type::Comma});

    if (current.isSuccess() &&
        std::find(targets.begin(), targets.end(),
                  current.getValue()->getType()) != targets.end()) {
        auto _operator = Expression::Operator::FromToken(*current.getValue());
        EatAndReturnIfError(targets);
        DefineAndReturnIfError(right, parseListExpression());

        left = new Expression::Binary(left.getValue(), _operator,
                                      right.getValue());
    }

    return left;
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Expression::Expression*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Error*>*>
Beam::Text::Parser::Parser::parseAssignmentExpression() {
    DefineAndReturnIfError(left, parseLogicalOrExpression());
    auto targets = std::vector<Lexer::Token::Type>(
        {Lexer::Token::Type::Assign, Lexer::Token::Type::PlusAssign,
         Lexer::Token::Type::MinusAssign, Lexer::Token::Type::AsteriskAssign,
         Lexer::Token::Type::SlashAssign, Lexer::Token::Type::ModuloAssign,
         Lexer::Token::Type::AmpersandAssign, Lexer::Token::Type::PipeAssign,
         Lexer::Token::Type::CaretAssign,
         Lexer::Token::Type::DoubleLessThanAssign,
         Lexer::Token::Type::DoubleGreaterThanAssign});

    if (current.isSuccess() &&
        std::find(targets.begin(), targets.end(),
                  current.getValue()->getType()) != targets.end()) {
        auto _operator = Expression::Operator::FromToken(*current.getValue());
        EatAndReturnIfError(targets);
        DefineAndReturnIfError(right, parseListExpression());

        left = new Expression::Binary(left.getValue(), _operator,
                                      right.getValue());
    }

    return left;
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Expression::Expression*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Error*>*>
Beam::Text::Parser::Parser::parseLogicalOrExpression() {
    DefineAndReturnIfError(left, parseLogicalAndExpression());
    auto targets =
        std::vector<Lexer::Token::Type>({Lexer::Token::Type::DoublePipe});

    if (current.isSuccess() &&
        std::find(targets.begin(), targets.end(),
                  current.getValue()->getType()) != targets.end()) {
        auto _operator = Expression::Operator::FromToken(*current.getValue());
        EatAndReturnIfError(targets);
        DefineAndReturnIfError(right, parseListExpression());

        left = new Expression::Binary(left.getValue(), _operator,
                                      right.getValue());
    }

    return left;
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Expression::Expression*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Error*>*>
Beam::Text::Parser::Parser::parseLogicalAndExpression() {
    DefineAndReturnIfError(left, parseBitwiseOrExpression());
    auto targets =
        std::vector<Lexer::Token::Type>({Lexer::Token::Type::DoubleAmpersand});

    if (current.isSuccess() &&
        std::find(targets.begin(), targets.end(),
                  current.getValue()->getType()) != targets.end()) {
        auto _operator = Expression::Operator::FromToken(*current.getValue());
        EatAndReturnIfError(targets);
        DefineAndReturnIfError(right, parseListExpression());

        left = new Expression::Binary(left.getValue(), _operator,
                                      right.getValue());
    }

    return left;
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Expression::Expression*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Error*>*>
Beam::Text::Parser::Parser::parseBitwiseOrExpression() {
    DefineAndReturnIfError(left, parseBitwiseXorExpression());
    auto targets = std::vector<Lexer::Token::Type>({Lexer::Token::Type::Pipe});

    if (current.isSuccess() &&
        std::find(targets.begin(), targets.end(),
                  current.getValue()->getType()) != targets.end()) {
        auto _operator = Expression::Operator::FromToken(*current.getValue());
        EatAndReturnIfError(targets);
        DefineAndReturnIfError(right, parseListExpression());

        left = new Expression::Binary(left.getValue(), _operator,
                                      right.getValue());
    }

    return left;
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Expression::Expression*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Error*>*>
Beam::Text::Parser::Parser::parseBitwiseXorExpression() {
    DefineAndReturnIfError(left, parseBitwiseAndExpression());
    auto targets = std::vector<Lexer::Token::Type>({Lexer::Token::Type::Caret});

    if (current.isSuccess() &&
        std::find(targets.begin(), targets.end(),
                  current.getValue()->getType()) != targets.end()) {
        auto _operator = Expression::Operator::FromToken(*current.getValue());
        EatAndReturnIfError(targets);
        DefineAndReturnIfError(right, parseListExpression());

        left = new Expression::Binary(left.getValue(), _operator,
                                      right.getValue());
    }

    return left;
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Expression::Expression*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Error*>*>
Beam::Text::Parser::Parser::parseBitwiseAndExpression() {
    DefineAndReturnIfError(left, parseEqualityExpression());
    auto targets =
        std::vector<Lexer::Token::Type>({Lexer::Token::Type::Ampersand});

    if (current.isSuccess() &&
        std::find(targets.begin(), targets.end(),
                  current.getValue()->getType()) != targets.end()) {
        auto _operator = Expression::Operator::FromToken(*current.getValue());
        EatAndReturnIfError(targets);
        DefineAndReturnIfError(right, parseListExpression());

        left = new Expression::Binary(left.getValue(), _operator,
                                      right.getValue());
    }

    return left;
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Expression::Expression*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Error*>*>
Beam::Text::Parser::Parser::parseEqualityExpression() {
    DefineAndReturnIfError(left, parseRelationalExpression());
    auto targets = std::vector<Lexer::Token::Type>(
        {Lexer::Token::Type::DoubleAssign,
         Lexer::Token::Type::ExclamationAssign});

    if (current.isSuccess() &&
        std::find(targets.begin(), targets.end(),
                  current.getValue()->getType()) != targets.end()) {
        auto _operator = Expression::Operator::FromToken(*current.getValue());
        EatAndReturnIfError(targets);
        DefineAndReturnIfError(right, parseListExpression());

        left = new Expression::Binary(left.getValue(), _operator,
                                      right.getValue());
    }

    return left;
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Expression::Expression*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Error*>*>
Beam::Text::Parser::Parser::parseRelationalExpression() {
    DefineAndReturnIfError(left, parseShiftExpression());
    auto targets = std::vector<Lexer::Token::Type>(
        {Lexer::Token::Type::LessThan, Lexer::Token::Type::GreaterThan,
         Lexer::Token::Type::LessThanAssign,
         Lexer::Token::Type::GreaterThanAssign});

    if (current.isSuccess() &&
        std::find(targets.begin(), targets.end(),
                  current.getValue()->getType()) != targets.end()) {
        auto _operator = Expression::Operator::FromToken(*current.getValue());
        EatAndReturnIfError(targets);
        DefineAndReturnIfError(right, parseListExpression());

        left = new Expression::Binary(left.getValue(), _operator,
                                      right.getValue());
    }

    return left;
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Expression::Expression*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Error*>*>
Beam::Text::Parser::Parser::parseShiftExpression() {
    DefineAndReturnIfError(left, parseAdditiveExpression());
    auto targets = std::vector<Lexer::Token::Type>(
        {Lexer::Token::Type::DoubleLessThan,
         Lexer::Token::Type::DoubleGreaterThan});

    if (current.isSuccess() &&
        std::find(targets.begin(), targets.end(),
                  current.getValue()->getType()) != targets.end()) {
        auto _operator = Expression::Operator::FromToken(*current.getValue());
        EatAndReturnIfError(targets);
        DefineAndReturnIfError(right, parseListExpression());

        left = new Expression::Binary(left.getValue(), _operator,
                                      right.getValue());
    }

    return left;
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Expression::Expression*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Error*>*>
Beam::Text::Parser::Parser::parseAdditiveExpression() {
    DefineAndReturnIfError(left, parseMultiplicativeExpression());
    auto targets = std::vector<Lexer::Token::Type>(
        {Lexer::Token::Type::Plus, Lexer::Token::Type::Minus});

    if (current.isSuccess() &&
        std::find(targets.begin(), targets.end(),
                  current.getValue()->getType()) != targets.end()) {
        auto _operator = Expression::Operator::FromToken(*current.getValue());
        EatAndReturnIfError(targets);
        DefineAndReturnIfError(right, parseListExpression());

        left = new Expression::Binary(left.getValue(), _operator,
                                      right.getValue());
    }

    return left;
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Expression::Expression*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Error*>*>
Beam::Text::Parser::Parser::parseMultiplicativeExpression() {
    DefineAndReturnIfError(left, parseUnaryExpression());
    auto targets = std::vector<Lexer::Token::Type>(
        {Lexer::Token::Type::Asterisk, Lexer::Token::Type::Slash,
         Lexer::Token::Type::Modulo});

    if (current.isSuccess() &&
        std::find(targets.begin(), targets.end(),
                  current.getValue()->getType()) != targets.end()) {
        auto _operator = Expression::Operator::FromToken(*current.getValue());
        EatAndReturnIfError(targets);
        DefineAndReturnIfError(right, parseListExpression());

        left = new Expression::Binary(left.getValue(), _operator,
                                      right.getValue());
    }

    return left;
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Expression::Expression*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Error*>*>
Beam::Text::Parser::Parser::parseUnaryExpression() {
    auto _operator = current;

    auto targets = std::vector<Lexer::Token::Type>(
        {Lexer::Token::Type::Plus, Lexer::Token::Type::Minus,
         Lexer::Token::Type::Exclamation, Lexer::Token::Type ::Tilde});

    if (std::find(targets.begin(), targets.end(),
                  _operator.getValue()->getType()) != targets.end()) {
        EatAndReturnIfError(targets);

        return new Expression::Unary(
            parsePostfixExpression().getValue(),
            Expression::Operator::FromToken(*_operator.getValue()));
    }

    return parsePostfixExpression();
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Expression::Expression*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Error*>*>
Beam::Text::Parser::Parser::parsePostfixExpression() {
    auto expression = parseLiteralExpression();
    return expression; // not implemented yet
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Expression::Expression*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Error*>*>
Beam::Text::Parser::Parser::parseLiteralExpression() {
    switch (current.getValue()->getType()) {
        case Beam::Text::Lexer::Token::Type::Identifier: {
            auto value = current.getValue()->getValue();
            EatAndReturnIfError({current.getValue()->getType()});
            return Expression::Literal::Name(value);
        }

        case Beam::Text::Lexer::Token::Type::Integer: {
            auto value = current.getValue()->getValue();
            EatAndReturnIfError({current.getValue()->getType()});
            return Expression::Literal::Integer(std::stoi(value));
        }

        case Beam::Text::Lexer::Token::Type::Float: {
            auto value = current.getValue()->getValue();
            EatAndReturnIfError({current.getValue()->getType()});
            return Expression::Literal::Float(std::stof(value));
        }

        case Beam::Text::Lexer::Token::Type::Character: {
            auto value = current.getValue()->getValue();
            EatAndReturnIfError({current.getValue()->getType()});
            return Expression::Literal::Character(value.at(0));
        }

        case Beam::Text::Lexer::Token::Type::String: {
            auto value = current.getValue()->getValue();
            EatAndReturnIfError({current.getValue()->getType()});
            return Expression::Literal::String(value);
        }

        case Beam::Text::Lexer::Token::Type::LeftParenthesis: {
            EatAndReturnIfError({current.getValue()->getType()});
            auto expression = parseListExpression();
            EatAndReturnIfError({Lexer::Token::Type::RightParenthesis});
            return expression;
        }

        default:
            return new Vec(
                Diagnostic::Error*,
                {new Diagnostic::Error(
                    Diagnostic::Error::Type::ErrorTypeUnexpectedToken,
                    Diagnostic::Error::Icon::ErrorIconCurlyBracesCross,
                    lexer->span, "other cases are not implemented yet.")});
    }
}

Beam::Diagnostic::DiResult<Beam::Text::Lexer::Token*, Beam::Diagnostic::Error*>
Beam::Text::Parser::Parser::eat(const std::vector<Lexer::Token::Type>& types) {
    if (std::find(types.begin(), types.end(), current.getValue()->getType()) ==
        types.end()) {
        auto typesAsFormatString = Vec(IO::Format::Types::String*, {});

        for (auto type : types) {
            typesAsFormatString.push_back(new IO::Format::Types::String(
                Lexer::Token::getTypeAsString(type)));
        }

        return currentError = new Diagnostic::Error(
                   Diagnostic::Error::Type::ErrorTypeUnexpectedToken,
                   Diagnostic::Error::Icon::ErrorIconCurlyBracesCross,
                   lexer->span,
                   "unexpected token " +
                       (current.getValue()->getValue().empty()
                            ? current.getValue()->getTypeAsString()
                            : '`' + current.getValue()->getValue() + '`') +
                       ". " +
                       (typesAsFormatString.size() > 1
                            ? "expected one of these types"
                            : "expected") +
                       ": " + typesAsFormatString.formatNoBraces());
    }

    return current = lexer->lexNext();
}

Beam::Diagnostic::DiResult<Beam::Text::Lexer::Token*, Beam::Diagnostic::Error*>
Beam::Text::Parser::Parser::skip() {
    return current = lexer->lexNext();
}

bool Beam::Text::Parser::Parser::currentIsReservedWord() const {
    auto reservedWords = std::vector({"if", "else", "while", "return", "let"});

    return current.isSuccess() &&
           current.getValue()->getType() == Lexer::Token::Type::Identifier &&
           std::find(reservedWords.begin(), reservedWords.end(),
                     current.getValue()->getValue()) != reservedWords.end();
}
