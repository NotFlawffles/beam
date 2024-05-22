#include <iostream>

#include "../../../include/beam/compiler/compiler.hpp"
#include "../../../include/beam/text/lexer/lexer.hpp"

Beam::Diagnostic::Result<Beam::Debug::Types::Vector<Beam::Diagnostic::Error*>*>
Beam::Compiler::compile(const Beam::IO::File::Source& source) {
    if (source.getReader().isFailure()) {
        return source.getReader().getError();
    }

    auto lexer = Beam::Text::Lexer::Lexer(source);
    auto errors = new Debug::Types::Vector<Diagnostic::Error*>({});

    while (true) {
        auto current = lexer.lexNext();

        if (current.isFailure()) {
            errors->push_back(current.getError());
        } else {
            if (current.getValue()->getType() ==
                Beam::Text::Lexer::Token::Type::EndOfFile) {
                break;
            }
        }
    }

    return errors;
}
