#include "../../../../include/beam/text/lexer/token.hpp"

std::string Beam::Text::Lexer::Token::getTypeAsString() const {
    switch (type) {
        case Type::Identifier:
            return "Identifier";

        case Type::Integer:
            return "Integer";

        case Type::Float:
            return "Float";

        case Type::Character:
            return "Character";

        case Type::String:
            return "String";

        case Type::Plus:
            return "+";

        case Type::Minus:
            return "-";

        case Type::Asterisk:
            return "*";

        case Type::Slash:
            return "/";

        case Type::Modulo:
            return "%";

        case Type::Ampersand:
            return "&";

        case Type::Pipe:
            return "|";

        case Type::Caret:
            return "^";

        case Type::Tilde:
            return "~";

        case Type::DoubleLessThan:
            return "<<";

        case Type::DoubleGreaterThan:
            return ">>";

        case Type::Exclamation:
            return "!";

        case Type::Assign:
            return "=";

        case Type::ExclamationAssign:
            return "!=";

        case Type::DoubleAssign:
            return "==";

        case Type::DoubleAmpersand:
            return "&&";

        case Type::DoublePipe:
            return "||";

        case Type::LessThan:
            return "<";

        case Type::GreaterThan:
            return ">";

        case Type::LessThanAssign:
            return "<=";

        case Type::GreaterThanAssign:
            return ">=";

        case Type::PlusAssign:
            return "+=";

        case Type::MinusAssign:
            return "-=";

        case Type::AsteriskAssign:
            return "*=";

        case Type::SlashAssign:
            return "/=";

        case Type::ModuloAssign:
            return "%=";

        case Type::AmpersandAssign:
            return "&=";

        case Type::PipeAssign:
            return "|=";

        case Type::CaretAssign:
            return "^=";

        case Type::DoubleLessThanAssign:
            return "<<=";

        case Type::DoubleGreaterThanAssign:
            return ">>=";

        case Type::LeftParenthesis:
            return "(";

        case Type::RightParenthesis:
            return ")";

        case Type::LeftCurlyBrace:
            return "{";

        case Type::RightCurlyBrace:
            return "}";

        case Type::Comma:
            return ",";

        case Type::Colon:
            return ":";

        case Type::SemiColon:
            return ";";

        case Type::Unhandled:
            return "Type::Unhandled";

        case Type::EndOfFile:
            return "Type::EndOfFile";

        default:
            return "Unreachable";
    }
}

std::string Beam::Text::Lexer::Token::format() {
    return getTypeAsString() + ": " + getValue();
}

std::string Beam::Text::Lexer::Token::debug() {
    return "Token(type: " + getTypeAsString() + ", span: " + span.debug() +
           ", value: \"" + getValue() + "\")";
}
