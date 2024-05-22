#include <cctype>
#include <functional>
#include <unordered_map>

#include "../../../../include/beam/diagnostic/error.hpp"
#include "../../../../include/beam/text/lexer/lexer.hpp"

Beam::Diagnostic::Result<Beam::Text::Lexer::Token*>
Beam::Text::Lexer::Lexer::lexNext() {
    switch (skipWhitespace()) {
        case '_':
            return lexIdentifier();

        case '.':
            return lexNumber();

        case '\'':
            return lexCharacter();

        case '\"':
            return lexString();

        case '+':
            return lexPlus();

        case '-':
            return lexMinus();

        case '*':
            return lexAsterisk();

        case '/':
            return lexSlash();

        case '%':
            return lexModulo();

        case '&':
            return lexAmpersand();

        case '|':
            return lexPipe();

        case '^':
            return lexCaret();

        case '~':
            return lexTilde();

        case '<':
            return lexLessThan();

        case '>':
            return lexGreaterThan();

        case '!':
            return lexExclamation();

        case '=':
            return lexAssign();

        case ',':
            return lexComma();

        case ':':
            return lexColon();

        case ';':
            return lexSemiColon();

        case '(':
        case ')':
        case '{':
        case '}':
            return lexBrace();

        case '\0':
            return lexEndOfFile();

        default:
            if (std::isalpha(current)) {
                return lexIdentifier();
            } else if (std::isdigit(current)) {
                return lexNumber();
            } else {
                return lexUnhandled();
            }
    }
}

Beam::Diagnostic::Result<Beam::Text::Lexer::Token*>
Beam::Text::Lexer::Lexer::lexIdentifier() {
    auto value = std::string();
    auto start = span.clone();

    while (std::isalnum(current) || current == '_') {
        value.push_back(current);
        advance();
    }

    return new Token(Token::Type::Identifier,
                     start.withLengthOf(value.length()), value);
}

Beam::Diagnostic::Result<Beam::Text::Lexer::Token*>
Beam::Text::Lexer::Lexer::lexNumber() {
    auto value = std::string();
    auto start = span.clone();
    auto type = current == '.' ? Token::Type::Float : Token::Type::Integer;

    while (std::isdigit(current) || current == '.') {
        if (current == '.') {
            if (type == Token::Type::Float) {
                return advanceWithResult(new Diagnostic::Error(
                    Diagnostic::Error::Type::ErrorTypeInvalidSyntax,
                    Diagnostic::Error::Icon::ErrorIconProgramCross,
                    span.clone().withLengthOf(value.length() + 1), "odd '.'"));
            }

            type = Token::Type::Float;
        }

        value.push_back(current);
        advance();
    }

    if (value == "." || (value.length() && value.at(value.length() - 1) == '.'))
        return new Diagnostic::Error(
            Diagnostic::Error::Type::ErrorTypeInvalidSyntax,
            Diagnostic::Error::Icon::ErrorIconProgramCross,
            span.clone()
                .withColumnOf(*span.getColumn() - 1)
                .withLengthOf(value.length()),
            "odd '.'");

    return new Token(type, start.withLengthOf(value.length()), value);
}

Beam::Diagnostic::Result<Beam::Text::Lexer::Token*>
Beam::Text::Lexer::Lexer::lexCharacter() {
    char value;
    auto start = span.clone();
    auto length = 2;

    if (advance() == '\\') {
        auto escapeSequence = lexEscapeSequence();

        if (escapeSequence.isFailure()) {
            return advanceWithResult(escapeSequence.getError());
        }

        value = escapeSequence.getValue()->debug().at(0);
        length++;
    } else {
        value = current;
    }

    if (advance() != '\'') {
        return new Diagnostic::Error(
            Diagnostic::Error::Type::ErrorTypeInvalidSyntax,
            Diagnostic::Error::Icon::ErrorIconProgramCross,
            start.withLengthOf(length), "unterminated character sequence");
    }

    return new Token(Token::Type::Character, start.withLengthOf(length),
                     {value});
}

Beam::Diagnostic::Result<Beam::Text::Lexer::Token*>
Beam::Text::Lexer::Lexer::lexString() {
    auto value = std::string();
    auto start = span.clone();

    while (advance() != '\"') {
        if (current == '\0') {
            return new Diagnostic::Error(
                Diagnostic::Error::Type::ErrorTypeInvalidSyntax,
                Diagnostic::Error::Icon::ErrorIconProgramCross,
                start.withLengthOf(value.length()),
                "unterminated string sequence");
        } else if (current == '\\') {
            auto escapeSequence = lexEscapeSequence();

            if (escapeSequence.isFailure()) {
                return escapeSequence.getError();
            }

            value.push_back(escapeSequence.getValue()->debug().at(0));
        } else {
            value.push_back(current);
        }
    }

    return advanceWithResult(new Token(
        Token::Type::String, start.withLengthOf(value.length()), value));
}

Beam::Diagnostic::Result<Beam::Text::Lexer::Token*>
Beam::Text::Lexer::Lexer::lexPlus() {
    auto start = span.clone().withLengthOf(1);

    switch (advance()) {
        case '=':
            return advanceWithResult(
                new Token(Token::Type::PlusAssign, start.withLengthOf(2)));

        default:
            return new Token(Token::Type::Plus, start);
    }
}

Beam::Diagnostic::Result<Beam::Text::Lexer::Token*>
Beam::Text::Lexer::Lexer::lexMinus() {
    auto start = span.clone().withLengthOf(1);

    switch (advance()) {
        case '=':
            return advanceWithResult(
                new Token(Token::Type::MinusAssign, start.withLengthOf(2)));

        default:
            return new Token(Token::Type::Minus, start);
    }
}

Beam::Diagnostic::Result<Beam::Text::Lexer::Token*>
Beam::Text::Lexer::Lexer::lexAsterisk() {
    auto start = span.clone().withLengthOf(1);

    switch (advance()) {
        case '=':
            return advanceWithResult(
                new Token(Token::Type::AsteriskAssign, start.withLengthOf(2)));

        default:
            return new Token(Token::Type::Asterisk, start);
    }
}

Beam::Diagnostic::Result<Beam::Text::Lexer::Token*>
Beam::Text::Lexer::Lexer::lexSlash() {
    auto start = span.clone().withLengthOf(1);

    switch (advance()) {
        case '=':
            return advanceWithResult(
                new Token(Token::Type::SlashAssign, start.withLengthOf(2)));

        case '/':
            while (advance() != '\n')
                ;
            return lexNext();

        case '*': {
            auto start = span.clone();

            while (true) {
                if (advance() == '*') {
                    if (advance() == '/') {
                        break;
                    }
                } else if (current == '\0') {
                    return new Diagnostic::Error(
                        Diagnostic::Error::Type::ErrorTypeInvalidSyntax,
                        Diagnostic::Error::Icon::ErrorIconProgramCross,
                        start.withLengthOf(*start.getColumn() -
                                           *span.getColumn()),
                        "unterminated multi-line comment sequence");
                } else if (current == '\n') {
                    *span.getRow() += 1;
                    *span.getColumn() = 0;
                }
            }

            return lexNext();
        }

        default:
            return new Token(Token::Type::Slash, start);
    }
}

Beam::Diagnostic::Result<Beam::Text::Lexer::Token*>
Beam::Text::Lexer::Lexer::lexModulo() {
    auto start = span.clone().withLengthOf(1);

    switch (advance()) {
        case '=':
            return advanceWithResult(
                new Token(Token::Type::ModuloAssign, start.withLengthOf(2)));

        default:
            return new Token(Token::Type::Modulo, start);
    }
}

Beam::Diagnostic::Result<Beam::Text::Lexer::Token*>
Beam::Text::Lexer::Lexer::lexAmpersand() {
    auto start = span.clone().withLengthOf(1);

    switch (advance()) {
        case '=':
            return advanceWithResult(
                new Token(Token::Type::AmpersandAssign, start.withLengthOf(2)));

        case '&':
            return advanceWithResult(
                new Token(Token::Type::DoubleAmpersand, start.withLengthOf(2)));

        default:
            return new Token(Token::Type::Ampersand, start);
    }
}

Beam::Diagnostic::Result<Beam::Text::Lexer::Token*>
Beam::Text::Lexer::Lexer::lexPipe() {
    auto start = span.clone().withLengthOf(1);

    switch (advance()) {
        case '=':
            return advanceWithResult(
                new Token(Token::Type::PipeAssign, start.withLengthOf(2)));

        case '|':
            return advanceWithResult(
                new Token(Token::Type::DoublePipe, start.withLengthOf(2)));

        default:
            return new Token(Token::Type::Pipe, start);
    }
}

Beam::Diagnostic::Result<Beam::Text::Lexer::Token*>
Beam::Text::Lexer::Lexer::lexCaret() {
    auto start = span.clone().withLengthOf(1);

    switch (advance()) {
        case '=':
            return advanceWithResult(
                new Token(Token::Type::CaretAssign, start.withLengthOf(2)));

        default:
            return new Token(Token::Type::Caret, start);
    }
}

Beam::Diagnostic::Result<Beam::Text::Lexer::Token*>
Beam::Text::Lexer::Lexer::lexTilde() {
    return advanceWithResult(new Token(Token::Type::Tilde, span));
}

Beam::Diagnostic::Result<Beam::Text::Lexer::Token*>
Beam::Text::Lexer::Lexer::lexLessThan() {
    auto start = span.clone().withLengthOf(1);

    switch (advance()) {
        case '=':
            return advanceWithResult(
                new Token(Token::Type::LessThanAssign, start.withLengthOf(2)));

        case '<':
            switch (advance()) {
                case '=':
                    return advanceWithResult(
                        new Token(Token::Type::DoubleLessThanAssign,
                                  start.withLengthOf(3)));

                default:
                    return new Token(Token::Type::DoubleLessThan,
                                     span.withLengthOf(2));
            }

        default:
            return new Token(Token::Type::LessThan, start);
    }
}

Beam::Diagnostic::Result<Beam::Text::Lexer::Token*>
Beam::Text::Lexer::Lexer::lexGreaterThan() {
    auto start = span.clone().withLengthOf(1);

    switch (advance()) {
        case '=':
            return advanceWithResult(new Token(Token::Type::GreaterThanAssign,
                                               start.withLengthOf(2)));

        case '>':
            switch (advance()) {
                case '=':
                    return advanceWithResult(
                        new Token(Token::Type::DoubleGreaterThanAssign,
                                  start.withLengthOf(3)));

                default:
                    return new Token(Token::Type::DoubleGreaterThan,
                                     span.withLengthOf(2));
            }

        default:
            return new Token(Token::Type::GreaterThan, start);
    }
}

Beam::Diagnostic::Result<Beam::Text::Lexer::Token*>
Beam::Text::Lexer::Lexer::lexExclamation() {
    auto start = span.clone().withLengthOf(1);

    switch (advance()) {
        case '=':
            return advanceWithResult(new Token(Token::Type::ExclamationAssign,
                                               start.withLengthOf(2)));

        default:
            return new Token(Token::Type::Exclamation, start);
    }
}

Beam::Diagnostic::Result<Beam::Text::Lexer::Token*>
Beam::Text::Lexer::Lexer::lexAssign() {
    auto start = span.clone().withLengthOf(1);

    switch (advance()) {
        case '=':
            return advanceWithResult(
                new Token(Token::Type::DoubleAssign, start.withLengthOf(2)));

        default:
            return new Token(Token::Type::Assign, start);
    }
}

Beam::Diagnostic::Result<Beam::Text::Lexer::Token*>
Beam::Text::Lexer::Lexer::lexBrace() {
    switch (current) {
        case '(':
            return advanceWithResult(
                new Token(Token::Type::LeftParenthesis, span));

        case ')':
            return advanceWithResult(
                new Token(Token::Type::RightParenthesis, span));

        case '{':
            return advanceWithResult(
                new Token(Token::Type::LeftCurlyBrace, span));

        case '}':
            return advanceWithResult(
                new Token(Token::Type::RightCurlyBrace, span));

        default:
            return advanceWithResult(new Token(Token::Type::Unhandled, span));
    }
}

Beam::Diagnostic::Result<Beam::Text::Lexer::Token*>
Beam::Text::Lexer::Lexer::lexComma() {
    return advanceWithResult(new Token(Token::Type::Comma, span));
}

Beam::Diagnostic::Result<Beam::Text::Lexer::Token*>
Beam::Text::Lexer::Lexer::lexColon() {
    return advanceWithResult(new Token(Token::Type::Colon, span));
}

Beam::Diagnostic::Result<Beam::Text::Lexer::Token*>
Beam::Text::Lexer::Lexer::lexSemiColon() {
    return advanceWithResult(new Token(Token::Type::SemiColon, span));
}

Beam::Diagnostic::Result<Beam::Text::Lexer::Token*>
Beam::Text::Lexer::Lexer::lexUnhandled() {
    return advanceWithResult(
        new Token(Token::Type::Unhandled, span, {current}));
}

Beam::Diagnostic::Result<Beam::Text::Lexer::Token*>
Beam::Text::Lexer::Lexer::lexEndOfFile() {
    return new Token(Token::Type::EndOfFile, span);
}

Beam::Diagnostic::Result<Beam::Text::Lexer::Token*>
Beam::Text::Lexer::Lexer::advanceWithResult(
    const Beam::Diagnostic::Result<Beam::Text::Lexer::Token*>& result) {
    advance();
    return result;
}

Beam::Diagnostic::Result<Beam::Debug::Types::Char*>
Beam::Text::Lexer::Lexer::lexEscapeSequence() {
    return std::unordered_map<char, Debug::Types::Char*> {
        {'a', new Debug::Types::Char('\a')},
        {'b', new Debug::Types::Char('\b')},
        {'f', new Debug::Types::Char('\f')},
        {'n', new Debug::Types::Char('\n')},
        {'r', new Debug::Types::Char('\r')},
        {'v', new Debug::Types::Char('\v')},
        {'\\', new Debug::Types::Char('\\')},
        {'\'', new Debug::Types::Char('\'')},
        {'\"', new Debug::Types::Char('\"')},
        {'?', new Debug::Types::Char('\?')}}
        .at(advance());
}

char Beam::Text::Lexer::Lexer::skipWhitespace() {
    while (std::isspace(current)) {
        if (current == '\n') {
            *span.getRow() += 1;
            *span.getColumn() = 0;
        }

        advance();
    }

    return current;
}

char Beam::Text::Lexer::Lexer::advance() {
    *span.getColumn() += 1;

    return current =
               source.getReader().getValue()->readAll()[*span.getIndex() += 1];
}
