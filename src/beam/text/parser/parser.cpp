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
#include "../../../../include/beam/text/parser/syntax/annotation/declaration/type/name.hpp"
#include "../../../../include/beam/text/parser/syntax/annotation/declaration/type/pointer.hpp"
#include "../../../../include/beam/text/parser/syntax/primary/block.hpp"
#include "../../../../include/beam/text/parser/syntax/primary/expression.hpp"
#include "../../../../include/beam/text/parser/syntax/statement/conditional/do.hpp"
#include "../../../../include/beam/text/parser/syntax/statement/conditional/if.hpp"
#include "../../../../include/beam/text/parser/syntax/statement/conditional/while.hpp"
#include "../../../../include/beam/text/parser/syntax/statement/declaration/function/function.hpp"
#include "../../../../include/beam/text/parser/syntax/statement/declaration/variable.hpp"
#include "../../../../include/beam/text/parser/syntax/statement/return.hpp"

#define EatAndReturnIfError(types)                                             \
    if (eat(types).isFailure())                                                \
    return new Vec(Beam::Diagnostic::Diagnostic*, {currentError})

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Syntax::AbstractSyntaxTree*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Parser::Parser::parse() {
    return parseBlock(Lexer::Token::Type::EndOfFile);
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Syntax::AbstractSyntaxTree*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Parser::Parser::parseNext() {
    if (current.isFailure()) {
        return new Vec(Diagnostic::Diagnostic*, {current.getError()});
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
        case Lexer::Token::Type::Asterisk:
        case Lexer::Token::Type::Exclamation:
        case Lexer::Token::Type::Tilde:
        case Lexer::Token::Type::LeftParenthesis:
            return parseExpression();

        case Lexer::Token::Type::LeftCurlyBrace:
            return parseBlock(Lexer::Token::Type::RightCurlyBrace);

        default:
            EatAndReturnIfError({current.getValue()->getType()});

            return new Vec(
                Diagnostic::Diagnostic*,
                {new Diagnostic::Error(
                    Diagnostic::Error::Type::ErrorTypeUnexpectedToken,
                    Diagnostic::Error::Icon::ErrorIconCurlyBracesCross,
                    current.getValue()->getSpan(), "expected statement.")});
    }
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Syntax::AbstractSyntaxTree*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Parser::Parser::parseBlock(const Lexer::Token::Type& until) {
    auto tree = Vec(Syntax::AbstractSyntaxTree*, {});
    auto errors = new Vec(Diagnostic::Diagnostic*, {});

    if (until != Lexer::Token::Type::EndOfFile) {
        EatAndReturnIfError({Lexer::Token::Type::LeftCurlyBrace});
    }

    while (true) {
        if (current.isSuccess() && current.getValue()->getType() == until) {
            break;
        }

        auto currentIsFailure = current.isFailure();
        auto next = parseNext();

        if (next.isSuccess()) {
            if (!Compiler::Utils::isDynamicallyCastableTo<
                    Syntax::AbstractSyntaxTree*, Syntax::Primary::Block*>(
                    next.getValue()) &&
                !Compiler::Utils::isDynamicallyCastableTo<
                    Syntax::AbstractSyntaxTree*,
                    Syntax::Statement::Conditional::Conditional*>(
                    next.getValue()) &&
                !Compiler::Utils::isDynamicallyCastableTo<
                    Syntax::AbstractSyntaxTree*,
                    Syntax::Statement::Declaration::Function::Function*>(
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

            if (currentIsFailure) {
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
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Parser::Parser::parseName() {
    if (currentIsReservedWord()) {
        return parseReservedWord();
    }

    return parseExpression();
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Syntax::AbstractSyntaxTree*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Parser::Parser::parseReservedWord() {
    if (current.isFailure()) {
        return new Vec(Diagnostic::Diagnostic*, {current.getError()});
    }

    if (current.getValue()->getValue() == "if") {
        return parseIfStatement();
    } else if (current.getValue()->getValue() == "while") {
        return parseWhileStatement();
    } else if (current.getValue()->getValue() == "return") {
        return parseReturnStatement();
    } else if (current.getValue()->getValue() == "let") {
        return parseDeclaration();
    } else if (current.getValue()->getValue() == "do") {
        return parseDoStatement();
    }

    DefineAndReturnVecIfErrorAndSkip(_, Diagnostic::Diagnostic*,
                                     expect({"statement"}));
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Syntax::AbstractSyntaxTree*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
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

        DefineAndReturnIfError(conditionResult, parseExpression());

        condition = dynamic_cast<Syntax::Primary::Expression*>(
            conditionResult.getValue());
    }

    DefineAndReturnIfError(body,
                           parseBlock(Lexer::Token::Type::RightCurlyBrace));

    Syntax::Statement::Conditional::If* elseClause = nullptr;

    if (!confirmedElse &&
        (currentIsReservedWord() && current.getValue()->getValue() == "else")) {
        DefineAndReturnIfError(result, parseIfStatement());

        elseClause = dynamic_cast<Syntax::Statement::Conditional::If*>(
            result.getValue());
    }

    return new Syntax::Statement::Conditional::If(
        condition, dynamic_cast<Syntax::Primary::Block*>(body.getValue()),
        elseClause);
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Syntax::AbstractSyntaxTree*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Parser::Parser::parseWhileStatement() {
    EatAndReturnIfError({Lexer::Token::Type::Identifier});
    DefineAndReturnIfError(condition, parseExpression());

    DefineAndReturnIfError(body,
                           parseBlock(Lexer::Token::Type::RightCurlyBrace));

    return new Syntax::Statement::Conditional::While(
        dynamic_cast<Syntax::Primary::Expression*>(condition.getValue()),
        dynamic_cast<Syntax::Primary::Block*>(body.getValue()));
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Syntax::AbstractSyntaxTree*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Parser::Parser::parseReturnStatement() {
    EatAndReturnIfError({Lexer::Token::Type::Identifier});
    DefineAndReturnIfError(expression, parseExpression());

    return new Syntax::Statement::Return(
        dynamic_cast<Syntax::Primary::Expression*>(expression.getValue()));
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Syntax::AbstractSyntaxTree*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Parser::Parser::parseAnnotationType() {
    DefineAndReturnIfError(name, getName());

    auto type = new Syntax::Annotation::Declaration::Type::Type(
        Syntax::Annotation::Declaration::Type::Type::Kind::Name);

    type = new Syntax::Annotation::Declaration::Type::Name(
        name.getValue(), name.getValue()->getSpan());

    EatAndReturnIfError({Lexer::Token::Type::Identifier});

    while (current.isSuccess() &&
           current.getValue()->getType() == Lexer::Token::Type::Asterisk) {
        type = new Syntax::Annotation::Declaration::Type::Pointer(type);
        EatAndReturnIfError({Lexer::Token::Type::Asterisk});
    }

    return type;
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Syntax::AbstractSyntaxTree*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Parser::Parser::parseDeclaration() {
    EatAndReturnIfError({Lexer::Token::Type::Identifier});

    if (current.isFailure()) {
        return new Vec(Diagnostic::Diagnostic*, {current.getError()});
    }

    auto flags = new IO::Format::Types::Char(
        Syntax::Annotation::Declaration::Flags::Private |
        Syntax::Annotation::Declaration::Flags::Immutable);

    if (currentIsReservedWord()) {
        DefineAndReturnIfError(flagsParsing, parseDeclarationFlags());
        flags = flagsParsing.getValue();
    }

    DefineAndReturnIfError(name, getName());
    EatAndReturnIfError({Lexer::Token::Type::Identifier});

    if (current.isFailure()) {
        return new Vec(Diagnostic::Diagnostic*, {current.getError()});
    }

    if (current.getValue()->getType() == Lexer::Token::Type::LeftParenthesis) {
        return parseFunctionDeclaration(*name.getValue(), flags);
    }

    return parseVariableDeclaration(*name.getValue(), flags);
}

Beam::Diagnostic::DiResult<
    Beam::IO::Format::Types::Char*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Parser::Parser::parseDeclarationFlags() {
    auto expectations =
        std::vector<std::string>({"pub", "priv", "mut", "imut"});

    auto flags = 0;

    DefineAndReturnVecIfError(expection, Diagnostic::Diagnostic*,
                              expect(expectations));

    if (current.getValue()->getValue() == "pub") {
        expectations.erase(
            std::find(expectations.begin(), expectations.end(), "pub"));

        expectations.erase(
            std::find(expectations.begin(), expectations.end(), "priv"));

        flags |= Syntax::Annotation::Declaration::Flags::Public;
    } else if (current.getValue()->getValue() == "mut") {
        expectations.erase(
            std::find(expectations.begin(), expectations.end(), "mut"));

        expectations.erase(
            std::find(expectations.begin(), expectations.end(), "imut"));

        flags |= Syntax::Annotation::Declaration::Flags::Mutable;
    } else if (current.getValue()->getValue() == "priv") {
        expectations.erase(
            std::find(expectations.begin(), expectations.end(), "priv"));

        expectations.erase(
            std::find(expectations.begin(), expectations.end(), "pub"));

        flags |= Syntax::Annotation::Declaration::Flags::Private;
    } else if (current.getValue()->getValue() == "imut") {
        expectations.erase(
            std::find(expectations.begin(), expectations.end(), "imut"));

        expectations.erase(
            std::find(expectations.begin(), expectations.end(), "mut"));

        flags |= Syntax::Annotation::Declaration::Flags::Immutable;
    }

    EatAndReturnIfError({Lexer::Token::Type::Identifier});

    if (currentIsReservedWord()) {
        DefineAndReturnVecIfErrorAndSkip(_, Diagnostic::Diagnostic*,
                                         expect(expectations));
    } else {
        return new IO::Format::Types::Char(flags);
    }

    if (current.getValue()->getValue() == "pub") {
        if (flags & (Syntax::Annotation::Declaration::Flags::Public |
                     Syntax::Annotation::Declaration::Flags::Private)) {
            DefineAndReturnVecIfErrorAndSkip(_, Diagnostic::Diagnostic*,
                                             expect(expectations));
        }

        flags |= Syntax::Annotation::Declaration::Flags::Public;
    } else if (current.getValue()->getValue() == "mut") {
        if (flags & (Syntax::Annotation::Declaration::Flags::Mutable |
                     Syntax::Annotation::Declaration::Flags::Immutable)) {
            DefineAndReturnVecIfErrorAndSkip(_, Diagnostic::Diagnostic*,
                                             expect(expectations));
        }

        flags |= Syntax::Annotation::Declaration::Flags::Mutable;
    } else if (current.getValue()->getValue() == "priv") {
        if (flags & (Syntax::Annotation::Declaration::Flags::Private |
                     Syntax::Annotation::Declaration::Flags::Public)) {
            DefineAndReturnVecIfErrorAndSkip(_, Diagnostic::Diagnostic*,
                                             expect(expectations));
        }

        flags |= Syntax::Annotation::Declaration::Flags::Private;
    } else if (current.getValue()->getValue() == "imut") {
        if (flags & (Syntax::Annotation::Declaration::Flags::Immutable |
                     Syntax::Annotation::Declaration::Flags::Mutable)) {
            DefineAndReturnVecIfErrorAndSkip(_, Diagnostic::Diagnostic*,
                                             expect(expectations));
        }

        flags |= Syntax::Annotation::Declaration::Flags::Immutable;
    }

    EatAndReturnIfError({Lexer::Token::Type::Identifier});
    return new IO::Format::Types::Char(flags);
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Syntax::AbstractSyntaxTree*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Parser::Parser::parseVariableDeclaration(
    const IO::Format::Types::String& name, IO::Format::Types::Char* flags) {
    EatAndReturnIfError({Lexer::Token::Type::Colon});
    DefineAndReturnIfError(anotationType, parseAnnotationType());
    EatAndReturnIfError({Lexer::Token::Type::Assign});
    DefineAndReturnIfError(value, parseExpression());

    return new Syntax::Statement::Declaration::Variable(
        dynamic_cast<Syntax::Annotation::Declaration::Type::Type*>(
            anotationType.getValue()),
        new Syntax::Annotation::Declaration::Flags(flags), name,
        dynamic_cast<Syntax::Primary::Expression*>(value.getValue()));
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Syntax::AbstractSyntaxTree*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Parser::Parser::parseFunctionDeclaration(
    const IO::Format::Types::String& name, IO::Format::Types::Char* flags) {
    DefineAndReturnIfError(parameters, parseFunctionParameters());
    EatAndReturnIfError({Lexer::Token::Type::Colon});
    DefineAndReturnIfError(type, parseAnnotationType());

    DefineAndReturnIfError(body,
                           parseBlock(Lexer::Token::Type::RightCurlyBrace));

    return new Syntax::Statement::Declaration::Function::Function(
        dynamic_cast<Syntax::Annotation::Declaration::Type::Type*>(
            type.getValue()),
        new Syntax::Annotation::Declaration::Flags(flags), name,
        parameters.getValue(),
        dynamic_cast<Syntax::Primary::Block*>(body.getValue()));
}

Beam::Diagnostic::DiResult<
    Beam::IO::Format::Types::Vector<Beam::Text::Parser::Syntax::Statement::
                                        Declaration::Function::Parameter*>*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Parser::Parser::parseFunctionParameters() {
    auto tree =
        new Vec(Syntax::Statement::Declaration::Function::Parameter*, {});
    auto errors = new Vec(Diagnostic::Diagnostic*, {});

    skip();

    while (true) {
        if (current.isSuccess() && current.getValue()->getType() ==
                                       Lexer::Token::Type::RightParenthesis) {
            break;
        }

        bool currentTokenIsFailure = current.isFailure();

        if (currentTokenIsFailure) {
            errors->push_back(current.getError());
            skip();
            continue;
        }

        auto flags = new IO::Format::Types::Char(
            Syntax::Annotation::Declaration::Flags::Private |
            Syntax::Annotation::Declaration::Flags::Immutable);

        if (currentIsReservedWord()) {
            auto flagsParsing = parseDeclarationFlags();

            if (flagsParsing.isFailure()) {
                errors->insert(errors->end(), flagsParsing.getError()->begin(),
                               flagsParsing.getError()->end());

                skip();
            } else {
                flags = flagsParsing.getValue();
            }
        }

        auto nameParsing = getName();
        auto name = std::string();

        if (nameParsing.isFailure()) {
            errors->insert(errors->end(), nameParsing.getError()->begin(),
                           nameParsing.getError()->end());

            skip();
        } else {
            name = nameParsing.getValue()->get();
        }

        auto nameExpectation = eat({Lexer::Token::Type::Identifier});

        if (nameExpectation.isFailure()) {
            errors->push_back(nameExpectation.getError());
            skip();
            continue;
        }

        auto colonExpectation = eat({Lexer::Token::Type::Colon});

        if (colonExpectation.isFailure()) {
            errors->push_back(colonExpectation.getError());
            skip();
            continue;
        }

        if (current.isFailure()) {
            errors->push_back(current.getError());
            skip();
            continue;
        }

        auto type = parseAnnotationType();

        if (type.isFailure()) {
            errors->insert(errors->end(), type.getError()->begin(),
                           type.getError()->end());
            skip();
            continue;
        }

        tree->push_back(new Syntax::Statement::Declaration::Function::Parameter(
            dynamic_cast<Syntax::Annotation::Declaration::Type::Type*>(
                type.getValue()),
            new Syntax::Annotation::Declaration::Flags(flags), name));

        if (current.isSuccess() && current.getValue()->getType() !=
                                       Lexer::Token::Type::RightParenthesis) {
            auto commaExpectation = eat({Lexer::Token::Type::Comma});

            if (commaExpectation.isFailure()) {
                errors->push_back(commaExpectation.getError());
                skip();
            }
        }
    }

    auto rightParenthesisExpectation =
        eat({Lexer::Token::Type::RightParenthesis});

    if (rightParenthesisExpectation.isFailure()) {
        errors->push_back(rightParenthesisExpectation.getError());
        skip();
    }

    if (errors->empty()) {
        return tree;
    } else {
        return errors;
    }
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Syntax::AbstractSyntaxTree*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Parser::Parser::parseDoStatement() {
    EatAndReturnIfError({Lexer::Token::Type::Identifier});

    DefineAndReturnIfError(body,
                           parseBlock(Lexer::Token::Type::RightCurlyBrace));

    if (current.isFailure() || current.getValue()->getValue() != "while") {
        return new Vec(Diagnostic::Diagnostic*,
                       {new Diagnostic::Error(
                           Diagnostic::Error::Type::ErrorTypeUnexpectedToken,
                           Diagnostic::Error::Icon::ErrorIconCurlyBracesCross,
                           lexer->span, "expected `while` statement.")});
    }

    EatAndReturnIfError({Lexer::Token::Type::Identifier});
    DefineAndReturnIfError(condition, parseExpression());
    EatAndReturnIfError({Lexer::Token::Type::SemiColon});

    return new Syntax::Statement::Conditional::Do(
        dynamic_cast<Syntax::Primary::Expression*>(condition.getValue()),
        dynamic_cast<Syntax::Primary::Block*>(body.getValue()));
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Syntax::AbstractSyntaxTree*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Parser::Parser::parseExpression() {
    DefineAndReturnIfError(expression, parseListExpression());
    return new Syntax::Primary::Expression(expression.getValue());
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Expression::Expression*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Parser::Parser::parseListExpression() {
    auto leftSpan = current.getValue()->getSpan();
    DefineAndReturnIfError(left, parseAssignmentExpression());
    auto targets = std::vector<Lexer::Token::Type>({Lexer::Token::Type::Comma});

    if (current.isSuccess() &&
        std::find(targets.begin(), targets.end(),
                  current.getValue()->getType()) != targets.end()) {
        auto operatorSpan = current.getValue()->getSpan();
        auto _operator = Expression::Operator::FromToken(*current.getValue());
        EatAndReturnIfError(targets);
        auto rightSpan = current.getValue()->getSpan();
        DefineAndReturnIfError(right, parseListExpression());

        left =
            new Expression::Binary(left.getValue(), _operator, right.getValue(),
                                   leftSpan, rightSpan, operatorSpan);
    }

    return left;
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Expression::Expression*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Parser::Parser::parseAssignmentExpression() {
    auto leftSpan = current.getValue()->getSpan();
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
        auto operatorSpan = current.getValue()->getSpan();
        auto _operator = Expression::Operator::FromToken(*current.getValue());
        EatAndReturnIfError(targets);
        auto rightSpan = current.getValue()->getSpan();
        DefineAndReturnIfError(right, parseListExpression());

        left =
            new Expression::Binary(left.getValue(), _operator, right.getValue(),
                                   leftSpan, rightSpan, operatorSpan);
    }

    return left;
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Expression::Expression*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Parser::Parser::parseLogicalOrExpression() {
    auto leftSpan = current.getValue()->getSpan();
    DefineAndReturnIfError(left, parseLogicalAndExpression());
    auto targets =
        std::vector<Lexer::Token::Type>({Lexer::Token::Type::DoublePipe});

    if (current.isSuccess() &&
        std::find(targets.begin(), targets.end(),
                  current.getValue()->getType()) != targets.end()) {
        auto operatorSpan = current.getValue()->getSpan();
        auto _operator = Expression::Operator::FromToken(*current.getValue());
        EatAndReturnIfError(targets);
        auto rightSpan = current.getValue()->getSpan();
        DefineAndReturnIfError(right, parseListExpression());

        left =
            new Expression::Binary(left.getValue(), _operator, right.getValue(),
                                   leftSpan, rightSpan, operatorSpan);
    }

    return left;
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Expression::Expression*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Parser::Parser::parseLogicalAndExpression() {
    auto leftSpan = current.getValue()->getSpan();
    DefineAndReturnIfError(left, parseBitwiseOrExpression());
    auto targets =
        std::vector<Lexer::Token::Type>({Lexer::Token::Type::DoubleAmpersand});

    if (current.isSuccess() &&
        std::find(targets.begin(), targets.end(),
                  current.getValue()->getType()) != targets.end()) {
        auto operatorSpan = current.getValue()->getSpan();
        auto _operator = Expression::Operator::FromToken(*current.getValue());
        EatAndReturnIfError(targets);
        auto rightSpan = current.getValue()->getSpan();
        DefineAndReturnIfError(right, parseListExpression());

        left =
            new Expression::Binary(left.getValue(), _operator, right.getValue(),
                                   leftSpan, rightSpan, operatorSpan);
    }

    return left;
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Expression::Expression*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Parser::Parser::parseBitwiseOrExpression() {
    auto leftSpan = current.getValue()->getSpan();
    DefineAndReturnIfError(left, parseBitwiseXorExpression());
    auto targets = std::vector<Lexer::Token::Type>({Lexer::Token::Type::Pipe});

    if (current.isSuccess() &&
        std::find(targets.begin(), targets.end(),
                  current.getValue()->getType()) != targets.end()) {
        auto operatorSpan = current.getValue()->getSpan();
        auto _operator = Expression::Operator::FromToken(*current.getValue());
        EatAndReturnIfError(targets);
        auto rightSpan = current.getValue()->getSpan();
        DefineAndReturnIfError(right, parseListExpression());

        left =
            new Expression::Binary(left.getValue(), _operator, right.getValue(),
                                   leftSpan, rightSpan, operatorSpan);
    }

    return left;
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Expression::Expression*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Parser::Parser::parseBitwiseXorExpression() {
    auto leftSpan = current.getValue()->getSpan();
    DefineAndReturnIfError(left, parseBitwiseAndExpression());
    auto targets = std::vector<Lexer::Token::Type>({Lexer::Token::Type::Caret});

    if (current.isSuccess() &&
        std::find(targets.begin(), targets.end(),
                  current.getValue()->getType()) != targets.end()) {
        auto operatorSpan = current.getValue()->getSpan();
        auto _operator = Expression::Operator::FromToken(*current.getValue());
        EatAndReturnIfError(targets);
        auto rightSpan = current.getValue()->getSpan();
        DefineAndReturnIfError(right, parseListExpression());

        left =
            new Expression::Binary(left.getValue(), _operator, right.getValue(),
                                   leftSpan, rightSpan, operatorSpan);
    }

    return left;
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Expression::Expression*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Parser::Parser::parseBitwiseAndExpression() {
    auto leftSpan = current.getValue()->getSpan();
    DefineAndReturnIfError(left, parseEqualityExpression());
    auto targets =
        std::vector<Lexer::Token::Type>({Lexer::Token::Type::Ampersand});

    if (current.isSuccess() &&
        std::find(targets.begin(), targets.end(),
                  current.getValue()->getType()) != targets.end()) {
        auto operatorSpan = current.getValue()->getSpan();
        auto _operator = Expression::Operator::FromToken(*current.getValue());
        EatAndReturnIfError(targets);
        auto rightSpan = current.getValue()->getSpan();
        DefineAndReturnIfError(right, parseListExpression());

        left =
            new Expression::Binary(left.getValue(), _operator, right.getValue(),
                                   leftSpan, rightSpan, operatorSpan);
    }

    return left;
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Expression::Expression*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Parser::Parser::parseEqualityExpression() {
    auto leftSpan = current.getValue()->getSpan();
    DefineAndReturnIfError(left, parseRelationalExpression());
    auto targets = std::vector<Lexer::Token::Type>(
        {Lexer::Token::Type::DoubleAssign,
         Lexer::Token::Type::ExclamationAssign});

    if (current.isSuccess() &&
        std::find(targets.begin(), targets.end(),
                  current.getValue()->getType()) != targets.end()) {
        auto operatorSpan = current.getValue()->getSpan();
        auto _operator = Expression::Operator::FromToken(*current.getValue());
        EatAndReturnIfError(targets);
        auto rightSpan = current.getValue()->getSpan();
        DefineAndReturnIfError(right, parseListExpression());

        left =
            new Expression::Binary(left.getValue(), _operator, right.getValue(),
                                   leftSpan, rightSpan, operatorSpan);
    }

    return left;
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Expression::Expression*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Parser::Parser::parseRelationalExpression() {
    auto leftSpan = current.getValue()->getSpan();
    DefineAndReturnIfError(left, parseShiftExpression());
    auto targets = std::vector<Lexer::Token::Type>(
        {Lexer::Token::Type::LessThan, Lexer::Token::Type::GreaterThan,
         Lexer::Token::Type::LessThanAssign,
         Lexer::Token::Type::GreaterThanAssign});

    if (current.isSuccess() &&
        std::find(targets.begin(), targets.end(),
                  current.getValue()->getType()) != targets.end()) {
        auto operatorSpan = current.getValue()->getSpan();
        auto _operator = Expression::Operator::FromToken(*current.getValue());
        EatAndReturnIfError(targets);
        auto rightSpan = current.getValue()->getSpan();
        DefineAndReturnIfError(right, parseListExpression());

        left =
            new Expression::Binary(left.getValue(), _operator, right.getValue(),
                                   leftSpan, rightSpan, operatorSpan);
    }

    return left;
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Expression::Expression*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Parser::Parser::parseShiftExpression() {
    auto leftSpan = current.getValue()->getSpan();
    DefineAndReturnIfError(left, parseAdditiveExpression());
    auto targets = std::vector<Lexer::Token::Type>(
        {Lexer::Token::Type::DoubleLessThan,
         Lexer::Token::Type::DoubleGreaterThan});

    if (current.isSuccess() &&
        std::find(targets.begin(), targets.end(),
                  current.getValue()->getType()) != targets.end()) {
        auto operatorSpan = current.getValue()->getSpan();
        auto _operator = Expression::Operator::FromToken(*current.getValue());
        EatAndReturnIfError(targets);
        auto rightSpan = current.getValue()->getSpan();
        DefineAndReturnIfError(right, parseListExpression());

        left =
            new Expression::Binary(left.getValue(), _operator, right.getValue(),
                                   leftSpan, rightSpan, operatorSpan);
    }

    return left;
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Expression::Expression*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Parser::Parser::parseAdditiveExpression() {
    auto leftSpan = current.getValue()->getSpan();
    DefineAndReturnIfError(left, parseMultiplicativeExpression());
    auto targets = std::vector<Lexer::Token::Type>(
        {Lexer::Token::Type::Plus, Lexer::Token::Type::Minus});

    if (current.isSuccess() &&
        std::find(targets.begin(), targets.end(),
                  current.getValue()->getType()) != targets.end()) {
        auto operatorSpan = current.getValue()->getSpan();
        auto _operator = Expression::Operator::FromToken(*current.getValue());
        EatAndReturnIfError(targets);
        auto rightSpan = current.getValue()->getSpan();
        DefineAndReturnIfError(right, parseListExpression());

        left =
            new Expression::Binary(left.getValue(), _operator, right.getValue(),
                                   leftSpan, rightSpan, operatorSpan);
    }

    return left;
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Expression::Expression*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Parser::Parser::parseMultiplicativeExpression() {
    auto leftSpan = current.getValue()->getSpan();
    DefineAndReturnIfError(left, parseUnaryExpression());
    auto targets = std::vector<Lexer::Token::Type>(
        {Lexer::Token::Type::Asterisk, Lexer::Token::Type::Slash,
         Lexer::Token::Type::Modulo});

    if (current.isSuccess() &&
        std::find(targets.begin(), targets.end(),
                  current.getValue()->getType()) != targets.end()) {
        auto operatorSpan = current.getValue()->getSpan();
        auto _operator = Expression::Operator::FromToken(*current.getValue());
        EatAndReturnIfError(targets);
        auto rightSpan = current.getValue()->getSpan();
        DefineAndReturnIfError(right, parseListExpression());

        left =
            new Expression::Binary(left.getValue(), _operator, right.getValue(),
                                   leftSpan, rightSpan, operatorSpan);
    }

    return left;
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Expression::Expression*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Parser::Parser::parseUnaryExpression() {
    auto operatorSpan = current.getValue()->getSpan();
    auto _operator = current;

    auto targets = std::vector<Lexer::Token::Type>(
        {Lexer::Token::Type::Plus, Lexer::Token::Type::Minus,
         Lexer::Token::Type::Asterisk, Lexer::Token::Type::Exclamation,
         Lexer::Token::Type ::Tilde});

    if (std::find(targets.begin(), targets.end(),
                  _operator.getValue()->getType()) != targets.end()) {
        EatAndReturnIfError(targets);

        auto expressionSpan = current.getValue()->getSpan();

        return new Expression::Unary(
            parsePostfixExpression().getValue(),
            Expression::Operator::FromToken(*_operator.getValue()),
            expressionSpan, operatorSpan);
    }

    return parsePostfixExpression();
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Expression::Expression*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Parser::Parser::parsePostfixExpression() {
    auto expression = parseLiteralExpression();
    return expression; // not implemented yet
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Expression::Expression*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Parser::Parser::parseLiteralExpression() {
    switch (current.getValue()->getType()) {
        case Beam::Text::Lexer::Token::Type::Identifier: {
            DefineAndReturnIfError(value, getName());
            auto span = current.getValue()->getSpan();
            EatAndReturnIfError({current.getValue()->getType()});
            return Expression::Literal::Name(value.getValue()->get(), span);
        }

        case Beam::Text::Lexer::Token::Type::Integer: {
            auto value = current.getValue()->getValue();
            auto span = current.getValue()->getSpan();
            EatAndReturnIfError({current.getValue()->getType()});
            return Expression::Literal::Integer(std::stoi(value), span);
        }

        case Beam::Text::Lexer::Token::Type::Float: {
            auto value = current.getValue()->getValue();
            auto span = current.getValue()->getSpan();
            EatAndReturnIfError({current.getValue()->getType()});
            return Expression::Literal::Float(std::stof(value), span);
        }

        case Beam::Text::Lexer::Token::Type::Character: {
            auto value = current.getValue()->getValue();
            auto span = current.getValue()->getSpan();
            EatAndReturnIfError({current.getValue()->getType()});
            return Expression::Literal::Character(value.at(0), span);
        }

        case Beam::Text::Lexer::Token::Type::String: {
            auto value = current.getValue()->getValue();
            auto span = current.getValue()->getSpan();
            EatAndReturnIfError({current.getValue()->getType()});
            return Expression::Literal::String(value, span);
        }

        case Beam::Text::Lexer::Token::Type::LeftParenthesis: {
            auto span = current.getValue()->getSpan();
            EatAndReturnIfError({current.getValue()->getType()});
            auto expression = parseListExpression();
            EatAndReturnIfError({Lexer::Token::Type::RightParenthesis});
            return expression;
        }

        default:
            return new Vec(
                Diagnostic::Diagnostic*,
                {new Diagnostic::Error(
                    Diagnostic::Error::Type::ErrorTypeUnexpectedToken,
                    Diagnostic::Error::Icon::ErrorIconCurlyBracesCross,
                    current.getValue()->getSpan(),
                    "other cases are not implemented yet.")});
    }
}

Beam::Diagnostic::DiResult<Beam::Text::Lexer::Token*,
                           Beam::Diagnostic::Diagnostic*>
Beam::Text::Parser::Parser::eat(const std::vector<Lexer::Token::Type>& types) {
    if (std::find(types.begin(), types.end(), current.getValue()->getType()) ==
        types.end()) {
        auto typesaAsFormatVector = Vec(IO::Format::Types::String*, {});

        for (const auto& type : types) {
            typesaAsFormatVector.push_back(new IO::Format::Types::String(
                Lexer::Token::getTypeAsString(type)));
        }

        return currentError = new Diagnostic::Error(
                   Diagnostic::Error::Type::ErrorTypeUnexpectedToken,
                   Diagnostic::Error::Icon::ErrorIconCurlyBracesCross,
                   current.getValue()->getSpan(),
                   "unexpected token " +
                       (current.getValue()->getValue().empty()
                            ? current.getValue()->getTypeAsString()
                            : '`' + current.getValue()->getValue() + '`') +
                       ". " +
                       (typesaAsFormatVector.size() > 1
                            ? "expected one of these types"
                            : "expected") +
                       ": " + typesaAsFormatVector.formatNoBraces());
    }

    return current = lexer->lexNext();
}

Beam::Diagnostic::DiResult<Beam::IO::Format::Types::String*,
                           Beam::Diagnostic::Diagnostic*>
Beam::Text::Parser::Parser::expect(
    const std::vector<std::string>& names) const {
    if (current.isFailure()) {
        return current.getError();
    }

    if (std::find(names.begin(), names.end(), current.getValue()->getValue()) !=
        names.end()) {
        return new IO::Format::Types::String(current.getValue()->getValue());
    }

    auto namesAsFormatVector = Vec(IO::Format::Types::String*, {});

    for (const auto& name : names) {
        namesAsFormatVector.push_back(new IO::Format::Types::String(name));
    }

    return new Diagnostic::Error(
        Diagnostic::Error::Type::ErrorTypeUnexpectedToken,
        Diagnostic::Error::Icon::ErrorIconCurlyBracesCross,
        current.getValue()->getSpan(),
        "unexpected token `" + current.getValue()->getValue() + "`. " +
            (namesAsFormatVector.size() > 1 ? "expected one of these tokens"
                                            : "expected") +
            ": " + namesAsFormatVector.formatNoBraces());
}

Beam::Diagnostic::DiResult<
    Beam::IO::Format::Types::String*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Parser::Parser::getName() const {
    if (current.isSuccess()) {
        if (currentIsReservedWord()) {
            DefineAndReturnVecIfError(expectation, Diagnostic::Diagnostic*,
                                      expect({"name"}));
        } else {
            return new IO::Format::Types::String(current.getValue()->getValue(),
                                                 current.getValue()->getSpan());
        }
    }

    return new Vec(Diagnostic::Diagnostic*, {current.getError()});
}

Beam::Diagnostic::DiResult<Beam::Text::Lexer::Token*,
                           Beam::Diagnostic::Diagnostic*>
Beam::Text::Parser::Parser::skip() {
    return current = lexer->lexNext();
}

bool Beam::Text::Parser::Parser::currentIsReservedWord() const {
    auto reservedWords = std::vector({"if", "else", "while", "do", "return",
                                      "let", "pub", "priv", "mut", "imut"});

    return current.isSuccess() &&
           current.getValue()->getType() == Lexer::Token::Type::Identifier &&
           std::find(reservedWords.begin(), reservedWords.end(),
                     current.getValue()->getValue()) != reservedWords.end();
}
