#include <cctype>
#include <functional>
#include <unordered_map>

#include "../../../../include/beam/diagnostic/error.hpp"
#include "../../../../include/beam/text/lexer/lexer.hpp"

Beam::Diagnostic::Result<Beam::Text::Lexer::Token>
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

Beam::Diagnostic::Result<Beam::Text::Lexer::Token>
Beam::Text::Lexer::Lexer::lexIdentifier() {
    auto value = std::string();
    auto start = span.clone();

    while (std::isalnum(current) || current == '_') {
        value.push_back(current);
        advance();
    }

    return Token(Token::Type::TokenTypeIdentifier,
                 start.withLengthOf(value.length()), value);
}

Beam::Diagnostic::Result<Beam::Text::Lexer::Token>
Beam::Text::Lexer::Lexer::lexNumber() {
    auto value = std::string();
    auto start = span.clone();
    auto type = current == '.' ? Token::Type::TokenTypeFloat
                               : Token::Type::TokenTypeInteger;

    while (std::isdigit(current) || current == '.') {
        if (current == '.') {
            if (type == Token::Type::TokenTypeFloat) {
                return advanceWithResult(Diagnostic::Error(
                    Diagnostic::Error::Type::ErrorTypeInvalidSyntax,
                    Diagnostic::Error::Icon::ErrorIconProgramCross,
                    span.clone().withLengthOf(value.length() + 1), "odd '.'"));
            }

            type = Token::Type::TokenTypeFloat;
        }

        value.push_back(current);
        advance();
    }

    if (value == "." || (value.length() && value.at(value.length() - 1) == '.'))
        return Diagnostic::Error(
            Diagnostic::Error::Type::ErrorTypeInvalidSyntax,
            Diagnostic::Error::Icon::ErrorIconProgramCross,
            span.clone()
                .withColumnOf(*span.getColumn() - 1)
                .withLengthOf(value.length()),
            "odd '.'");

    return Token(type, start.withLengthOf(value.length()), value);
}

Beam::Diagnostic::Result<Beam::Text::Lexer::Token>
Beam::Text::Lexer::Lexer::lexCharacter() {
    char value;
    auto start = span.clone();
    auto length = 2;

    if (advance() == '\\') {
        auto escapeSequence = lexEscapeSequence();

        if (escapeSequence.isFailure()) {
            return advanceWithResult(escapeSequence.getError());
        }

        value = escapeSequence.getValue().debug().at(0);
        length++;
    } else {
        value = current;
    }

    if (advance() != '\'') {
        return Diagnostic::Error(
            Diagnostic::Error::Type::ErrorTypeInvalidSyntax,
            Diagnostic::Error::Icon::ErrorIconProgramCross,
            start.withLengthOf(length), "unterminated character sequence");
    }

    return Token(Token::Type::TokenTypeCharacter, start.withLengthOf(length),
                 {value});
}

Beam::Diagnostic::Result<Beam::Text::Lexer::Token>
Beam::Text::Lexer::Lexer::lexString() {
    auto value = std::string();
    auto start = span.clone();

    while (advance() != '\"') {
        if (current == '\0') {
            return Diagnostic::Error(
                Diagnostic::Error::Type::ErrorTypeInvalidSyntax,
                Diagnostic::Error::Icon::ErrorIconProgramCross,
                start.withLengthOf(value.length()),
                "unterminated string sequence");
        } else if (current == '\\') {
            auto escapeSequence = lexEscapeSequence();

            if (escapeSequence.isFailure()) {
                return escapeSequence.getError();
            }

            value.push_back(escapeSequence.getValue().debug().at(0));
        } else {
            value.push_back(current);
        }
    }

    return advanceWithResult(Token(Token::Type::TokenTypeString,
                                   start.withLengthOf(value.length()), value));
}

Beam::Diagnostic::Result<Beam::Text::Lexer::Token>
Beam::Text::Lexer::Lexer::lexPlus() {
    auto start = span.clone().withLengthOf(1);

    switch (advance()) {
        case '=':
            return advanceWithResult(
                Token(Token::Type::TokenTypePlusAssign, start.withLengthOf(2)));

        default:
            return Token(Token::Type::TokenTypePlus, start);
    }
}

Beam::Diagnostic::Result<Beam::Text::Lexer::Token>
Beam::Text::Lexer::Lexer::lexMinus() {
    auto start = span.clone().withLengthOf(1);

    switch (advance()) {
        case '=':
            return advanceWithResult(Token(Token::Type::TokenTypeMinusAssign,
                                           start.withLengthOf(2)));

        default:
            return Token(Token::Type::TokenTypeMinus, start);
    }
}

Beam::Diagnostic::Result<Beam::Text::Lexer::Token>
Beam::Text::Lexer::Lexer::lexAsterisk() {
    auto start = span.clone().withLengthOf(1);

    switch (advance()) {
        case '=':
            return advanceWithResult(Token(Token::Type::TokenTypeAsteriskAssign,
                                           start.withLengthOf(2)));

        default:
            return Token(Token::Type::TokenTypeAsterisk, start);
    }
}

Beam::Diagnostic::Result<Beam::Text::Lexer::Token>
Beam::Text::Lexer::Lexer::lexSlash() {
    auto start = span.clone().withLengthOf(1);

    switch (advance()) {
        case '=':
            return advanceWithResult(Token(Token::Type::TokenTypeSlashAssign,
                                           start.withLengthOf(2)));

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
                    return Diagnostic::Error(
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
            return Token(Token::Type::TokenTypeSlash, start);
    }
}

Beam::Diagnostic::Result<Beam::Text::Lexer::Token>
Beam::Text::Lexer::Lexer::lexModulo() {
    auto start = span.clone().withLengthOf(1);

    switch (advance()) {
        case '=':
            return advanceWithResult(Token(Token::Type::TokenTypeModuloAssign,
                                           start.withLengthOf(2)));

        default:
            return Token(Token::Type::TokenTypeModulo, start);
    }
}

Beam::Diagnostic::Result<Beam::Text::Lexer::Token>
Beam::Text::Lexer::Lexer::lexAmpersand() {
    auto start = span.clone().withLengthOf(1);

    switch (advance()) {
        case '=':
            return advanceWithResult(Token(
                Token::Type::TokenTypeAmpersandAssign, start.withLengthOf(2)));

        case '&':
            return advanceWithResult(Token(
                Token::Type::TokenTypeDoubleAmpersand, start.withLengthOf(2)));

        default:
            return Token(Token::Type::TokenTypeAmpersand, start);
    }
}

Beam::Diagnostic::Result<Beam::Text::Lexer::Token>
Beam::Text::Lexer::Lexer::lexPipe() {
    auto start = span.clone().withLengthOf(1);

    switch (advance()) {
        case '=':
            return advanceWithResult(
                Token(Token::Type::TokenTypePipeAssign, start.withLengthOf(2)));

        case '|':
            return advanceWithResult(
                Token(Token::Type::TokenTypeDoublePipe, start.withLengthOf(2)));

        default:
            return Token(Token::Type::TokenTypePipe, start);
    }
}

Beam::Diagnostic::Result<Beam::Text::Lexer::Token>
Beam::Text::Lexer::Lexer::lexCaret() {
    auto start = span.clone().withLengthOf(1);

    switch (advance()) {
        case '=':
            return advanceWithResult(Token(Token::Type::TokenTypeCaretAssign,
                                           start.withLengthOf(2)));

        default:
            return Token(Token::Type::TokenTypeCaret, start);
    }
}

Beam::Diagnostic::Result<Beam::Text::Lexer::Token>
Beam::Text::Lexer::Lexer::lexTilde() {
    return advanceWithResult(Token(Token::Type::TokenTypeTilde, span));
}

Beam::Diagnostic::Result<Beam::Text::Lexer::Token>
Beam::Text::Lexer::Lexer::lexLessThan() {
    auto start = span.clone().withLengthOf(1);

    switch (advance()) {
        case '=':
            return advanceWithResult(Token(Token::Type::TokenTypeLessThanAssign,
                                           start.withLengthOf(2)));

        case '<':
            switch (advance()) {
                case '=':
                    return advanceWithResult(
                        Token(Token::Type::TokenTypeDoubleLessThanAssign,
                              start.withLengthOf(3)));

                default:
                    return Token(Token::Type::TokenTypeDoubleLessThan,
                                 span.withLengthOf(2));
            }

        default:
            return Token(Token::Type::TokenTypeLessThan, start);
    }
}

Beam::Diagnostic::Result<Beam::Text::Lexer::Token>
Beam::Text::Lexer::Lexer::lexGreaterThan() {
    auto start = span.clone().withLengthOf(1);

    switch (advance()) {
        case '=':
            return advanceWithResult(
                Token(Token::Type::TokenTypeGreaterThanAssign,
                      start.withLengthOf(2)));

        case '>':
            switch (advance()) {
                case '=':
                    return advanceWithResult(
                        Token(Token::Type::TokenTypeDoubleGreaterThanAssign,
                              start.withLengthOf(3)));

                default:
                    return Token(Token::Type::TokenTypeDoubleGreaterThan,
                                 span.withLengthOf(2));
            }

        default:
            return Token(Token::Type::TokenTypeGreaterThan, start);
    }
}

Beam::Diagnostic::Result<Beam::Text::Lexer::Token>
Beam::Text::Lexer::Lexer::lexExclamation() {
    auto start = span.clone().withLengthOf(1);

    switch (advance()) {
        case '=':
            return advanceWithResult(
                Token(Token::Type::TokenTypeExclamationAssign,
                      start.withLengthOf(2)));

        default:
            return Token(Token::Type::TokenTypeExclamation, start);
    }
}

Beam::Diagnostic::Result<Beam::Text::Lexer::Token>
Beam::Text::Lexer::Lexer::lexAssign() {
    auto start = span.clone().withLengthOf(1);

    switch (advance()) {
        case '=':
            return advanceWithResult(Token(Token::Type::TokenTypeDoubleAssign,
                                           start.withLengthOf(2)));

        default:
            return Token(Token::Type::TokenTypeAssign, start);
    }
}

Beam::Diagnostic::Result<Beam::Text::Lexer::Token>
Beam::Text::Lexer::Lexer::lexBrace() {
    switch (current) {
        case '(':
            return advanceWithResult(
                Token(Token::Type::TokenTypeLeftParenthesis, span));

        case ')':
            return advanceWithResult(
                Token(Token::Type::TokenTypeRightParenthesis, span));

        case '{':
            return advanceWithResult(
                Token(Token::Type::TokenTypeLeftCurlyBrace, span));

        case '}':
            return advanceWithResult(
                Token(Token::Type::TokenTypeRightCurlyBrace, span));

        default:
            return advanceWithResult(
                Token(Token::Type::TokenTypeUnhandled, span));
    }
}

Beam::Diagnostic::Result<Beam::Text::Lexer::Token>
Beam::Text::Lexer::Lexer::lexComma() {
    return advanceWithResult(Token(Token::Type::TokenTypeComma, span));
}

Beam::Diagnostic::Result<Beam::Text::Lexer::Token>
Beam::Text::Lexer::Lexer::lexColon() {
    return advanceWithResult(Token(Token::Type::TokenTypeColon, span));
}

Beam::Diagnostic::Result<Beam::Text::Lexer::Token>
Beam::Text::Lexer::Lexer::lexSemiColon() {
    return advanceWithResult(Token(Token::Type::TokenTypeSemiColon, span));
}

Beam::Diagnostic::Result<Beam::Text::Lexer::Token>
Beam::Text::Lexer::Lexer::lexUnhandled() {
    return advanceWithResult(
        Token(Token::Type::TokenTypeUnhandled, span, {current}));
}

Beam::Diagnostic::Result<Beam::Text::Lexer::Token>
Beam::Text::Lexer::Lexer::lexEndOfFile() {
    return Token(Token::Type::TokenTypeEndOfFile, span);
}

Beam::Diagnostic::Result<Beam::Text::Lexer::Token>
Beam::Text::Lexer::Lexer::advanceWithResult(
    const Beam::Diagnostic::Result<Beam::Text::Lexer::Token>& result) {
    advance();
    return result;
}

Beam::Diagnostic::Result<Beam::Debug::Types::Char>
Beam::Text::Lexer::Lexer::lexEscapeSequence() {
    return std::unordered_map<char, Debug::Types::Char> {
        {'a', '\a'}, {'b', '\b'},  {'f', '\f'},  {'n', '\n'},  {'r', '\r'},
        {'v', '\v'}, {'\\', '\\'}, {'\'', '\''}, {'\"', '\"'}, {'?', '\?'}}
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
               source.getReader().getValue().readAll()[*span.getIndex() += 1];
}
