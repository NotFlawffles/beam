#include "../../../../../include/beam/text/parser/syntax/ast.hpp"

std::string
Beam::Text::Parser::Syntax::AbstractSyntaxTree::getTypeAsString() const {
    switch (type) {
        case Type::Block:
            return "Block";

        case Type::Expression:
            return "Expression";

        default:
            return "Unreachable";
    }
}
