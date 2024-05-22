#include <iostream>

#include "../../../include/beam/compiler/compiler.hpp"
#include "../../../include/beam/text/lexer/lexer.hpp"
#include "../../../include/beam/text/parser/parser.hpp"

Beam::Diagnostic::Result<
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Error*>*,
    Beam::Diagnostic::Error*>
Beam::Compiler::compile(const Beam::IO::File::Source& source,
                        const bool& debug) {
    if (source.getReader().isFailure()) {
        return source.getReader().getError();
    }

    auto lexer = Beam::Text::Lexer::Lexer(source);
    auto parser = Beam::Text::Parser::Parser(&lexer);
    auto tree = parser.parse();

    std::cerr << (debug ? tree.debug() : tree.format()) << std::endl;

    return new IO::Format::Types::Vector<Diagnostic::Error*>({});
}
