#include <iostream>

#include "../../../include/beam/compiler/compiler.hpp"
#include "../../../include/beam/text/lexer/lexer.hpp"
#include "../../../include/beam/text/parser/parser.hpp"

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Syntax::AbstractSyntaxTree*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Error*>*>
Beam::Compiler::compile(const Beam::IO::File::Source& source) {
    if (source.getReader().isFailure()) {
        return new Vec(Diagnostic::Error*, {source.getReader().getError()});
    }

    auto lexer = Beam::Text::Lexer::Lexer(source);
    auto parser = Beam::Text::Parser::Parser(&lexer);
    auto tree = parser.parse();

    return tree;
}
