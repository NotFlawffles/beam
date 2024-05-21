#include "../../../../include/beam/text/lexer/token.hpp"

std::string Beam::Text::Lexer::Token::getTypeAsString() const {
    switch (type) {
        case TokenTypeIdentifier:
            return "Identifier";

        case TokenTypeInteger:
            return "Integer";

        case TokenTypeFloat:
            return "Float";

        case TokenTypeCharacter:
            return "Character";

        case TokenTypeString:
            return "String";

        case TokenTypePlus:
            return "+";

        case TokenTypeMinus:
            return "-";

        case TokenTypeAsterisk:
            return "*";

        case TokenTypeSlash:
            return "/";

        case TokenTypeModulo:
            return "%";

        case TokenTypeAmpersand:
            return "&";

        case TokenTypePipe:
            return "|";

        case TokenTypeCaret:
            return "^";

        case TokenTypeTilde:
            return "~";

        case TokenTypeDoubleLessThan:
            return "<<";

        case TokenTypeDoubleGreaterThan:
            return ">>";

        case TokenTypeExclamation:
            return "!";

        case TokenTypeAssign:
            return "=";

        case TokenTypeExclamationAssign:
            return "!=";

        case TokenTypeDoubleAssign:
            return "==";

        case TokenTypeDoubleAmpersand:
            return "&&";

        case TokenTypeDoublePipe:
            return "||";

        case TokenTypeLessThan:
            return "<";

        case TokenTypeGreaterThan:
            return ">";

        case TokenTypeLessThanAssign:
            return "<=";

        case TokenTypeGreaterThanAssign:
            return ">=";

        case TokenTypePlusAssign:
            return "+=";

        case TokenTypeMinusAssign:
            return "-=";

        case TokenTypeAsteriskAssign:
            return "*=";

        case TokenTypeSlashAssign:
            return "/=";

        case TokenTypeModuloAssign:
            return "%=";

        case TokenTypeAmpersandAssign:
            return "&=";

        case TokenTypePipeAssign:
            return "|=";

        case TokenTypeCaretAssign:
            return "^=";

        case TokenTypeDoubleLessThanAssign:
            return "<<=";

        case TokenTypeDoubleGreaterThanAssign:
            return ">>=";

        case TokenTypeLeftParenthesis:
            return "(";

        case TokenTypeRightParenthesis:
            return ")";

        case TokenTypeLeftCurlyBrace:
            return "{";

        case TokenTypeRightCurlyBrace:
            return "}";

        case TokenTypeComma:
            return ",";

        case TokenTypeColon:
            return ":";

        case TokenTypeSemiColon:
            return ";";

        case TokenTypeUnhandled:
            return "TokenTypeUnhandled";

        case TokenTypeEndOfFile:
            return "TokenTypeEndOfFile";

        default:
            return "Unreachable";
    }
}

std::string Beam::Text::Lexer::Token::debug() {
    return "Token(type: " + getTypeAsString() + ", span: " + span.debug() +
           ", value: \"" + getValue() + "\")";
}
