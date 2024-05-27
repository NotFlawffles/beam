#pragma once

#include "../../io/format/display.hpp"
#include "../../io/format/types/string.hpp"
#include "../../io/string/span.hpp"

namespace Beam::Text::Lexer {
class Token: IO::Format::Display {
  public:
    enum class Type {
        Identifier,
        Integer,
        Float,
        Character,
        String,

        Plus,
        Minus,
        Asterisk,
        Slash,
        Modulo,
        Ampersand,
        Pipe,
        Caret,
        Tilde,
        DoubleLessThan,
        DoubleGreaterThan,
        Exclamation,
        Assign,
        ExclamationAssign,
        DoubleAssign,
        DoubleAmpersand,
        DoublePipe,
        LessThan,
        GreaterThan,
        LessThanAssign,
        GreaterThanAssign,
        PlusAssign,
        MinusAssign,
        AsteriskAssign,
        SlashAssign,
        ModuloAssign,
        AmpersandAssign,
        PipeAssign,
        CaretAssign,
        DoubleLessThanAssign,
        DoubleGreaterThanAssign,

        LeftParenthesis,
        RightParenthesis,
        LeftCurlyBrace,
        RightCurlyBrace,

        Comma,
        Colon,
        SemiColon,

        Unhandled,
        EndOfFile
    };

    Token(const Type& type, const IO::String::Span& span,
          const std::string value)
        : type(type), span(span), value(value) {}

    Token(const Type& type, const IO::String::Span& span)
        : type(type), span(span) {}

    Token(): type(Type::Identifier) {}

    Type getType() const { return type; }

    IO::String::Span getSpan() const { return span; }

    std::string getValue() const { return value; }

    std::string getTypeAsString() const;
    static std::string getTypeAsString(const Token::Type& type);

    std::string format() override;

    std::string debug() override;

  private:
    Type type;
    IO::String::Span span;
    const std::string value;
};
} // namespace Beam::Text::Lexer
