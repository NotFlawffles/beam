#include <algorithm>
#include <array>
#include <memory>

#include "../../../../include/beam/text/parser/expression/binary.hpp"
#include "../../../../include/beam/text/parser/expression/literal.hpp"
#include "../../../../include/beam/text/parser/expression/unary.hpp"
#include "../../../../include/beam/text/parser/parser.hpp"
#include "../../../../include/beam/text/parser/syntax/block.hpp"
#include "../../../../include/beam/text/parser/syntax/expression.hpp"

#define DefineAndReturnIfError(name, value)                                    \
    auto name = value;                                                         \
    if (name.isFailure())                                                      \
        return name.getError();

Beam::Diagnostic::Result<
    Beam::Text::Parser::Syntax::AbstractSyntaxTree*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Error*>*>
Beam::Text::Parser::Parser::parse() {
    return parseBlock(Lexer::Token::Type::EndOfFile);
}

Beam::Diagnostic::Result<Beam::Text::Parser::Syntax::AbstractSyntaxTree*,
                         Beam::Diagnostic::Error*>
Beam::Text::Parser::Parser::parseNext() {
    if (current.isFailure()) {
        return current.getError();
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
            return parseExpression();

        default:
            eat({current.getValue()->getType()});

            return new Diagnostic::Error(
                Diagnostic::Error::Type::ErrorTypeUnexpectedToken,
                Diagnostic::Error::Icon::ErrorIconCurlyBracesCross, lexer->span,
                "other cases are not implemented yet.");
    }

    return new Diagnostic::Error(
        Diagnostic::Error::Type::ErrorTypeUnexpectedToken,
        Diagnostic::Error::Icon::ErrorIconCurlyBracesCross, lexer->span,
        "expected statement.");
}

Beam::Diagnostic::Result<Beam::Text::Parser::Syntax::AbstractSyntaxTree*,
                         Beam::Diagnostic::Error*>
Beam::Text::Parser::Parser::parseName() {
    if (currentIsReservedWord()) {
        // not implemented yet.
    }

    return parseExpression();
}

Beam::Diagnostic::Result<
    Beam::Text::Parser::Syntax::AbstractSyntaxTree*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Error*>*>
Beam::Text::Parser::Parser::parseBlock(const Lexer::Token::Type& until) {
    auto tree = IO::Format::Types::Vector<Syntax::AbstractSyntaxTree*>({});
    auto errors = new IO::Format::Types::Vector<Diagnostic::Error*>({});

    while (true) {
        if (current.isSuccess() && current.getValue()->getType() == until) {
            break;
        }

        auto next = parseNext();

        if (next.isSuccess()) {
            auto semiColonCheck = eat({Lexer::Token::Type::SemiColon});

            if (semiColonCheck.isFailure()) {
                errors->push_back(semiColonCheck.getError());
            }

            tree.push_back(next.getValue());
        } else {
            errors->push_back(next.getError());
        }
    }

    if (errors->empty()) {
        return new Syntax::Block(tree);
    } else {
        return errors;
    }
}

Beam::Diagnostic::Result<Beam::Text::Parser::Syntax::AbstractSyntaxTree*,
                         Beam::Diagnostic::Error*>
Beam::Text::Parser::Parser::parseExpression() {
    DefineAndReturnIfError(expression, parseListExpression());
    return new Syntax::Expression(expression.getValue());
}

Beam::Diagnostic::Result<Beam::Text::Parser::Expression::Expression*,
                         Beam::Diagnostic::Error*>
Beam::Text::Parser::Parser::parseListExpression() {
    DefineAndReturnIfError(left, parseAssignmentExpression());
    auto targets = std::vector<Lexer::Token::Type>({Lexer::Token::Type::Comma});

    if (current.isSuccess() &&
        std::find(targets.begin(), targets.end(),
                  current.getValue()->getType()) != targets.end()) {
        auto _operator = current.getValue();
        eat(targets);
        DefineAndReturnIfError(right, parseListExpression());

        left = new Expression::Binary(left.getValue(), *_operator,
                                      right.getValue());
    }

    return left;
}

Beam::Diagnostic::Result<Beam::Text::Parser::Expression::Expression*,
                         Beam::Diagnostic::Error*>
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
        auto _operator = current.getValue();
        eat(targets);
        DefineAndReturnIfError(right, parseListExpression());

        left = new Expression::Binary(left.getValue(), *_operator,
                                      right.getValue());
    }

    return left;
}

Beam::Diagnostic::Result<Beam::Text::Parser::Expression::Expression*,
                         Beam::Diagnostic::Error*>
Beam::Text::Parser::Parser::parseLogicalOrExpression() {
    DefineAndReturnIfError(left, parseLogicalAndExpression());
    auto targets =
        std::vector<Lexer::Token::Type>({Lexer::Token::Type::DoublePipe});

    if (current.isSuccess() &&
        std::find(targets.begin(), targets.end(),
                  current.getValue()->getType()) != targets.end()) {
        auto _operator = current.getValue();
        eat(targets);
        DefineAndReturnIfError(right, parseListExpression());

        left = new Expression::Binary(left.getValue(), *_operator,
                                      right.getValue());
    }

    return left;
}

Beam::Diagnostic::Result<Beam::Text::Parser::Expression::Expression*,
                         Beam::Diagnostic::Error*>
Beam::Text::Parser::Parser::parseLogicalAndExpression() {
    DefineAndReturnIfError(left, parseBitwiseOrExpression());
    auto targets =
        std::vector<Lexer::Token::Type>({Lexer::Token::Type::DoubleAmpersand});

    if (current.isSuccess() &&
        std::find(targets.begin(), targets.end(),
                  current.getValue()->getType()) != targets.end()) {
        auto _operator = current.getValue();
        eat(targets);
        DefineAndReturnIfError(right, parseListExpression());

        left = new Expression::Binary(left.getValue(), *_operator,
                                      right.getValue());
    }

    return left;
}

Beam::Diagnostic::Result<Beam::Text::Parser::Expression::Expression*,
                         Beam::Diagnostic::Error*>
Beam::Text::Parser::Parser::parseBitwiseOrExpression() {
    DefineAndReturnIfError(left, parseBitwiseXorExpression());
    auto targets = std::vector<Lexer::Token::Type>({Lexer::Token::Type::Pipe});

    if (current.isSuccess() &&
        std::find(targets.begin(), targets.end(),
                  current.getValue()->getType()) != targets.end()) {
        auto _operator = current.getValue();
        eat(targets);
        DefineAndReturnIfError(right, parseListExpression());

        left = new Expression::Binary(left.getValue(), *_operator,
                                      right.getValue());
    }

    return left;
}

Beam::Diagnostic::Result<Beam::Text::Parser::Expression::Expression*,
                         Beam::Diagnostic::Error*>
Beam::Text::Parser::Parser::parseBitwiseXorExpression() {
    DefineAndReturnIfError(left, parseBitwiseAndExpression());
    auto targets = std::vector<Lexer::Token::Type>({Lexer::Token::Type::Caret});

    if (current.isSuccess() &&
        std::find(targets.begin(), targets.end(),
                  current.getValue()->getType()) != targets.end()) {
        auto _operator = current.getValue();
        eat(targets);
        DefineAndReturnIfError(right, parseListExpression());

        left = new Expression::Binary(left.getValue(), *_operator,
                                      right.getValue());
    }

    return left;
}

Beam::Diagnostic::Result<Beam::Text::Parser::Expression::Expression*,
                         Beam::Diagnostic::Error*>
Beam::Text::Parser::Parser::parseBitwiseAndExpression() {
    DefineAndReturnIfError(left, parseEqualityExpression());
    auto targets =
        std::vector<Lexer::Token::Type>({Lexer::Token::Type::Ampersand});

    if (current.isSuccess() &&
        std::find(targets.begin(), targets.end(),
                  current.getValue()->getType()) != targets.end()) {
        auto _operator = current.getValue();
        eat(targets);
        DefineAndReturnIfError(right, parseListExpression());

        left = new Expression::Binary(left.getValue(), *_operator,
                                      right.getValue());
    }

    return left;
}

Beam::Diagnostic::Result<Beam::Text::Parser::Expression::Expression*,
                         Beam::Diagnostic::Error*>
Beam::Text::Parser::Parser::parseEqualityExpression() {
    DefineAndReturnIfError(left, parseRelationalExpression());
    auto targets = std::vector<Lexer::Token::Type>(
        {Lexer::Token::Type::DoubleAssign,
         Lexer::Token::Type::ExclamationAssign});

    if (current.isSuccess() &&
        std::find(targets.begin(), targets.end(),
                  current.getValue()->getType()) != targets.end()) {
        auto _operator = current.getValue();
        eat(targets);
        DefineAndReturnIfError(right, parseListExpression());

        left = new Expression::Binary(left.getValue(), *_operator,
                                      right.getValue());
    }

    return left;
}

Beam::Diagnostic::Result<Beam::Text::Parser::Expression::Expression*,
                         Beam::Diagnostic::Error*>
Beam::Text::Parser::Parser::parseRelationalExpression() {
    DefineAndReturnIfError(left, parseShiftExpression());
    auto targets = std::vector<Lexer::Token::Type>(
        {Lexer::Token::Type::LessThan, Lexer::Token::Type::GreaterThan,
         Lexer::Token::Type::LessThanAssign,
         Lexer::Token::Type::GreaterThanAssign});

    if (current.isSuccess() &&
        std::find(targets.begin(), targets.end(),
                  current.getValue()->getType()) != targets.end()) {
        auto _operator = current.getValue();
        eat(targets);
        DefineAndReturnIfError(right, parseListExpression());

        left = new Expression::Binary(left.getValue(), *_operator,
                                      right.getValue());
    }

    return left;
}

Beam::Diagnostic::Result<Beam::Text::Parser::Expression::Expression*,
                         Beam::Diagnostic::Error*>
Beam::Text::Parser::Parser::parseShiftExpression() {
    DefineAndReturnIfError(left, parseAdditiveExpression());
    auto targets = std::vector<Lexer::Token::Type>(
        {Lexer::Token::Type::DoubleLessThan,
         Lexer::Token::Type::DoubleGreaterThan});

    if (current.isSuccess() &&
        std::find(targets.begin(), targets.end(),
                  current.getValue()->getType()) != targets.end()) {
        auto _operator = current.getValue();
        eat(targets);
        DefineAndReturnIfError(right, parseListExpression());

        left = new Expression::Binary(left.getValue(), *_operator,
                                      right.getValue());
    }

    return left;
}

Beam::Diagnostic::Result<Beam::Text::Parser::Expression::Expression*,
                         Beam::Diagnostic::Error*>
Beam::Text::Parser::Parser::parseAdditiveExpression() {
    DefineAndReturnIfError(left, parseMultiplicativeExpression());
    auto targets = std::vector<Lexer::Token::Type>(
        {Lexer::Token::Type::Plus, Lexer::Token::Type::Minus});

    if (current.isSuccess() &&
        std::find(targets.begin(), targets.end(),
                  current.getValue()->getType()) != targets.end()) {
        auto _operator = current.getValue();
        eat(targets);
        DefineAndReturnIfError(right, parseListExpression());

        left = new Expression::Binary(left.getValue(), *_operator,
                                      right.getValue());
    }

    return left;
}

Beam::Diagnostic::Result<Beam::Text::Parser::Expression::Expression*,
                         Beam::Diagnostic::Error*>
Beam::Text::Parser::Parser::parseMultiplicativeExpression() {
    DefineAndReturnIfError(left, parseUnaryExpression());
    auto targets = std::vector<Lexer::Token::Type>(
        {Lexer::Token::Type::Asterisk, Lexer::Token::Type::Slash,
         Lexer::Token::Type::Modulo});

    if (current.isSuccess() &&
        std::find(targets.begin(), targets.end(),
                  current.getValue()->getType()) != targets.end()) {
        auto _operator = current.getValue();
        eat(targets);
        DefineAndReturnIfError(right, parseListExpression());

        left = new Expression::Binary(left.getValue(), *_operator,
                                      right.getValue());
    }

    return left;
}

Beam::Diagnostic::Result<Beam::Text::Parser::Expression::Expression*,
                         Beam::Diagnostic::Error*>
Beam::Text::Parser::Parser::parseUnaryExpression() {
    auto _operator = current;

    auto targets = std::vector<Lexer::Token::Type>(
        {Lexer::Token::Type::Plus, Lexer::Token::Type::Minus,
         Lexer::Token::Type::Exclamation, Lexer::Token::Type ::Tilde});

    if (std::find(targets.begin(), targets.end(),
                  _operator.getValue()->getType()) != targets.end()) {
        eat(targets);

        return new Expression::Unary(parsePostfixExpression().getValue(),
                                     *_operator.getValue());
    }

    return parsePostfixExpression();
}

Beam::Diagnostic::Result<Beam::Text::Parser::Expression::Expression*,
                         Beam::Diagnostic::Error*>
Beam::Text::Parser::Parser::parsePostfixExpression() {
    auto expression = parseLiteralExpression();
    return expression; // not implemented yet
}

Beam::Diagnostic::Result<Beam::Text::Parser::Expression::Expression*,
                         Beam::Diagnostic::Error*>
Beam::Text::Parser::Parser::parseLiteralExpression() {
    switch (current.getValue()->getType()) {
        case Beam::Text::Lexer::Token::Type::Identifier: {
            auto value = current.getValue()->getValue();
            eat({current.getValue()->getType()});
            return Expression::Literal::Name(value);
        }

        case Beam::Text::Lexer::Token::Type::Integer: {
            auto value = current.getValue()->getValue();
            eat({current.getValue()->getType()});
            return Expression::Literal::Integer(std::stoi(value));
        }

        case Beam::Text::Lexer::Token::Type::Float: {
            auto value = current.getValue()->getValue();
            eat({current.getValue()->getType()});
            return Expression::Literal::Float(std::stof(value));
        }

        case Beam::Text::Lexer::Token::Type::Character: {
            auto value = current.getValue()->getValue();
            eat({current.getValue()->getType()});
            return Expression::Literal::Character(value.at(0));
        }

        case Beam::Text::Lexer::Token::Type::String: {
            auto value = current.getValue()->getValue();
            eat({current.getValue()->getType()});
            return Expression::Literal::String(value);
        }

        default:
            return new Diagnostic::Error(
                Diagnostic::Error::Type::ErrorTypeUnexpectedToken,
                Diagnostic::Error::Icon::ErrorIconCurlyBracesCross, lexer->span,
                "other cases are not implemented yet.");
    }
}

Beam::Diagnostic::Result<Beam::Text::Lexer::Token*, Beam::Diagnostic::Error*>
Beam::Text::Parser::Parser::eat(const std::vector<Lexer::Token::Type>& types) {
    if (std::find(types.begin(), types.end(), current.getValue()->getType()) ==
        types.end()) {
        return new Diagnostic::Error(
            Diagnostic::Error::Type::ErrorTypeUnexpectedToken,
            Diagnostic::Error::Icon::ErrorIconCurlyBracesCross, lexer->span,
            "unexpected token `" + current.getValue()->getTypeAsString() +
                "`. expected: " +
                Lexer::Token(types.at(0), lexer->span, "").getTypeAsString());
    }

    return current = lexer->lexNext();
}

bool Beam::Text::Parser::Parser::currentIsReservedWord() const {
    auto reservedWords = std::vector({"let"});

    return current.isSuccess() &&
           current.getValue()->getType() == Lexer::Token::Type::Identifier &&
           std::find(reservedWords.begin(), reservedWords.end(),
                     current.getValue()->getValue()) != reservedWords.end();
}
