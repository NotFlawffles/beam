#include "../../../../../include/beam/text/parser/expression/operator.hpp"

Beam::Text::Parser::Expression::Operator*
Beam::Text::Parser::Expression::Operator::FromToken(const Lexer::Token& token) {
    switch (token.getType()) {
        case Lexer::Token::Type::Plus:
            return new Operator(Operator::Type::Plus, token.getTypeAsString());

        case Lexer::Token::Type::Minus:
            return new Operator(Operator::Type::Minus, token.getTypeAsString());

        case Lexer::Token::Type::Asterisk:
            return new Operator(Operator::Type::Multiply,
                                token.getTypeAsString());

        case Lexer::Token::Type::Slash:
            return new Operator(Operator::Type::Divide,
                                token.getTypeAsString());

        case Lexer::Token::Type::Modulo:
            return new Operator(Operator::Type::Remainder,
                                token.getTypeAsString());

        case Lexer::Token::Type::Ampersand:
            return new Operator(Operator::Type::BitwiseAnd,
                                token.getTypeAsString());

        case Lexer::Token::Type::Pipe:
            return new Operator(Operator::Type::BitwiseOr,
                                token.getTypeAsString());

        case Lexer::Token::Type::Caret:
            return new Operator(Operator::Type::BitwiseXor,
                                token.getTypeAsString());

        case Lexer::Token::Type::Tilde:
            return new Operator(Operator::Type::BitwiseNot,
                                token.getTypeAsString());

        case Lexer::Token::Type::DoubleLessThan:
            return new Operator(Operator::Type::BitwiseLeftShift,
                                token.getTypeAsString());

        case Lexer::Token::Type::DoubleGreaterThan:
            return new Operator(Operator::Type::BitwiseRightShift,
                                token.getTypeAsString());

        case Lexer::Token::Type::Exclamation:
            return new Operator(Operator::Type::LogicalNot,
                                token.getTypeAsString());

        case Lexer::Token::Type::Assign:
            return new Operator(Operator::Type::Assign,
                                token.getTypeAsString());

        case Lexer::Token::Type::ExclamationAssign:
            return new Operator(Operator::Type::NotEqual,
                                token.getTypeAsString());

        case Lexer::Token::Type::DoubleAssign:
            return new Operator(Operator::Type::Equal, token.getTypeAsString());

        case Lexer::Token::Type::DoubleAmpersand:
            return new Operator(Operator::Type::LogicalAnd,
                                token.getTypeAsString());

        case Lexer::Token::Type::DoublePipe:
            return new Operator(Operator::Type::LogicalOr,
                                token.getTypeAsString());

        case Lexer::Token::Type::LessThan:
            return new Operator(Operator::Type::LessThan,
                                token.getTypeAsString());

        case Lexer::Token::Type::GreaterThan:
            return new Operator(Operator::Type::GreaterThan,
                                token.getTypeAsString());

        case Lexer::Token::Type::LessThanAssign:
            return new Operator(Operator::Type::LessThanOrEqual,
                                token.getTypeAsString());

        case Lexer::Token::Type::GreaterThanAssign:
            return new Operator(Operator::Type::GreaterThanOrEqual,
                                token.getTypeAsString());

        case Lexer::Token::Type::PlusAssign:
            return new Operator(Operator::Type::PlusAssign,
                                token.getTypeAsString());

        case Lexer::Token::Type::MinusAssign:
            return new Operator(Operator::Type::MinusAssign,
                                token.getTypeAsString());

        case Lexer::Token::Type::AsteriskAssign:
            return new Operator(Operator::Type::MultiplyAssign,
                                token.getTypeAsString());

        case Lexer::Token::Type::SlashAssign:
            return new Operator(Operator::Type::DivideAssign,
                                token.getTypeAsString());

        case Lexer::Token::Type::ModuloAssign:
            return new Operator(Operator::Type::RemainderAssign,
                                token.getTypeAsString());

        case Lexer::Token::Type::AmpersandAssign:
            return new Operator(Operator::Type::BitwiseAndAssign,
                                token.getTypeAsString());

        case Lexer::Token::Type::PipeAssign:
            return new Operator(Operator::Type::BitwiseOrAssign,
                                token.getTypeAsString());

        case Lexer::Token::Type::CaretAssign:
            return new Operator(Operator::Type::BitwiseXorAssign,
                                token.getTypeAsString());

        case Lexer::Token::Type::DoubleLessThanAssign:
            return new Operator(Operator::Type::BitwiseLeftShiftAssign,
                                token.getTypeAsString());

        case Lexer::Token::Type::DoubleGreaterThanAssign:
            return new Operator(Operator::Type::BitwiseRightShiftAssign,
                                token.getTypeAsString());

        case Lexer::Token::Type::Comma:
            return new Operator(Operator::Type::Comma, token.getTypeAsString());

        default:
            return new Operator(Operator::Type::Invalid,
                                token.getTypeAsString());
    }
}

std::string Beam::Text::Parser::Expression::Operator::getTypeAsString() const {
    switch (getType()) {
        case Type::Plus:
            return "Plus";

        case Type::Minus:
            return "Minus";

        case Type::Multiply:
            return "Multiply";

        case Type::Divide:
            return "Divide";

        case Type::Remainder:
            return "Remainder";

        case Type::BitwiseAnd:
            return "BitwiseAnd";

        case Type::BitwiseOr:
            return "BitwiseOr";

        case Type::BitwiseXor:
            return "BitwiseXor";

        case Type::BitwiseNot:
            return "BitwiseNot";

        case Type::BitwiseLeftShift:
            return "BitwiseLeftShift";

        case Type::BitwiseRightShift:
            return "BitwiseRightShift";

        case Type::LogicalNot:
            return "LogicalNot";

        case Type::Assign:
            return "Assign";

        case Type::NotEqual:
            return "NotEqual";

        case Type::Equal:
            return "Equal";

        case Type::LogicalAnd:
            return "LogicalAnd";

        case Type::LogicalOr:
            return "LogicalOr";

        case Type::LessThan:
            return "LessThan";

        case Type::GreaterThan:
            return "GreaterThan";

        case Type::LessThanOrEqual:
            return "LessThanOrEqual";

        case Type::GreaterThanOrEqual:
            return "GreaterThanOrEqual";

        case Type::PlusAssign:
            return "PlusAssign";

        case Type::MinusAssign:
            return "MinusAssign";

        case Type::MultiplyAssign:
            return "MultiplyAssign";

        case Type::DivideAssign:
            return "DivideAssign";

        case Type::RemainderAssign:
            return "RemainderAssign";

        case Type::BitwiseAndAssign:
            return "BitwiseAndAssign";

        case Type::BitwiseOrAssign:
            return "BitwiseOrAssign";

        case Type::BitwiseXorAssign:
            return "BitwiseXorAssign";

        case Type::BitwiseLeftShiftAssign:
            return "BitwiseLeftShiftAssign";

        case Type::BitwiseRightShiftAssign:
            return "BitwiseRightShiftAssign";

        case Type::Comma:
            return "Comma";

        case Type::Invalid:
            return "Invalid";

        default:
            return "Unreachable";
    }
}

std::string Beam::Text::Parser::Expression::Operator::format() {
    return getValue();
}

std::string Beam::Text::Parser::Expression::Operator::debug() {
    return "Operator(type: " + getTypeAsString() + ", value: \"" + getValue() +
           "\")";
}
