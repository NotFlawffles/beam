#include "../../../../../include/beam/text/parser/syntax/ast.hpp"

std::string
Beam::Text::Parser::Syntax::AbstractSyntaxTree::getTypeAsString() const {
    switch (type) {
        case Type::Primary:
            return "Primary";

        case Type::Statement:
            return "Statement";

        default:
            return "Unreachable";
    }
}
