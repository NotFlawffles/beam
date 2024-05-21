#pragma once

#include "../../io/format/display/debugger.hpp"
#include "../../io/string/span.hpp"

namespace Beam::Text::Lexer {
class Token: IO::Format::Display::Debugger {
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

    Token(const Type& type, const IO::String::Span& span,
          const std::string value)
        : type(type), span(span), value(value) {}

    Token(const Type& type, const IO::String::Span& span)
        : type(type), span(span) {}

    Token(): type(Type::TokenTypeIdentifier) {}

    Type getType() const { return type; }

    std::string getValue() const { return value; }

    std::string getTypeAsString() const;

    std::string debug() override;

  private:
    const Type type;
    IO::String::Span span;
    const std::string value;
};
} // namespace Beam::Text::Lexer
