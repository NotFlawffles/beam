#pragma once

#include "../../io/display.hpp"
#include "../span.hpp"

namespace Beam::Text::Lexer {
class Token: IO::Display {
  public:
    enum Type {
        TokenTypeIdentifier,
        TokenTypeInteger,
        TokenTypeFloat,
        TokenTypeCharacter,
        TokenTypeString,

        TokenTypePlus,
        TokenTypeMinus,
        TokenTypeAsterisk,
        TokenTypeSlash,
        TokenTypeModulo,
        TokenTypeAmpersand,
        TokenTypePipe,
        TokenTypeCaret,
        TokenTypeTilde,
        TokenTypeDoubleLessThan,
        TokenTypeDoubleGreaterThan,
        TokenTypeExclamation,
        TokenTypeAssign,
        TokenTypeExclamationAssign,
        TokenTypeDoubleAssign,
        TokenTypeDoubleAmpersand,
        TokenTypeDoublePipe,
        TokenTypeLessThan,
        TokenTypeGreaterThan,
        TokenTypeLessThanAssign,
        TokenTypeGreaterThanAssign,
        TokenTypePlusAssign,
        TokenTypeMinusAssign,
        TokenTypeAsteriskAssign,
        TokenTypeSlashAssign,
        TokenTypeModuloAssign,
        TokenTypeAmpersandAssign,
        TokenTypePipeAssign,
        TokenTypeCaretAssign,
        TokenTypeDoubleLessThanAssign,
        TokenTypeDoubleGreaterThanAssign,

        TokenTypeLeftParenthesis,
        TokenTypeRightParenthesis,
        TokenTypeLeftCurlyBrace,
        TokenTypeRightCurlyBrace,

        TokenTypeComma,
        TokenTypeColon,
        TokenTypeSemiColon,

        TokenTypeUnhandled,
        TokenTypeEndOfFile
    };

    Token(const Type& type, const Text::Span& span, const std::string value)
        : type(type), span(span), value(value) {}

    Token(const Type& type, const Text::Span& span): type(type), span(span) {}

    Token(): type(Type::TokenTypeIdentifier) {}

    Type getType() const { return type; }

    std::string getValue() const { return value; }

    std::string getTypeAsString() const;

    std::string debug() override;

  private:
    const Type type;
    Span span;
    const std::string value;
};
} // namespace Beam::Text::Lexer
