#include "../../../../../../include/beam/text/parser/syntax/primary/block.hpp"

std::string Beam::Text::Parser::Syntax::Primary::Block::format() {
    return getValue().format();
}

std::string Beam::Text::Parser::Syntax::Primary::Block::debug() {
    return "Block(value: " + getValue().debug() + ')';
}
