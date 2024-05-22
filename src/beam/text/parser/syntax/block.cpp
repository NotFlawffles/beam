#include "../../../../../include/beam/text/parser/syntax/block.hpp"

std::string Beam::Text::Parser::Syntax::Block::format() {
    return getValue().format();
}

std::string Beam::Text::Parser::Syntax::Block::debug() {
    return "Block(value: " + getValue().debug() + ')';
}
