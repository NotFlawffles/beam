#include <iostream>

#include "../../../include/beam/compiler/compiler.hpp"
#include "../../../include/beam/text/lexer/lexer.hpp"

Beam::Diagnostic::Result<
    Beam::Text::Debug::DisplayableVector<Beam::Diagnostic::Error>>
Beam::Compiler::compile(const Beam::Text::Source& source) {
    if (source.getReader().isFailure()) {
        return source.getReader().getError();
    }

    auto lexer = Beam::Text::Lexer::Lexer(source);
    Text::Debug::DisplayableVector<Diagnostic::Error> errors;

    while (true) {
        auto current = lexer.lexNext();

        if (current.isFailure()) {
            errors.push_back(current.getError());
        } else {
            if (current.getValue().getType() ==
                Beam::Text::Lexer::Token::Type::TokenTypeEndOfFile) {
                break;
            }
        }
    }

    return errors;
}
