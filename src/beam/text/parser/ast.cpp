#include "../../../../include/beam/text/parser/ast.hpp"

std::string Beam::Text::Parser::Ast::getTypeAsString() const {
    switch (type) {
        case AstTypeBlock:
            return "AstTypeBlock";

        case AstTypeExpression:
            return "AstTypeExpression";

        default:
            return "Unreachable";
    }
}
