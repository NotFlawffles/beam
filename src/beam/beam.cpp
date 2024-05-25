#include "../../include/beam/compiler/compiler.hpp"
#include "../../include/beam/io/file/source.hpp"

#include "../../include/beam/io/format/colors.hpp"
#include "../../include/beam/text/parser/syntax/primary/block.hpp"

#include <iostream>

int main(int argc, char** argv) {
    (void) argc;

    auto source = Beam::IO::File::Source(argv[0], argv[1]);
    auto compilation = Beam::Compiler::compile(source);

    if (compilation.isSuccess()) {
        for (auto value :
             dynamic_cast<Beam::Text::Parser::Syntax::Primary::Block*>(
                 compilation.getValue())
                 ->getValue()) {
            std::cerr << value->format() << std::endl;
        }
    } else {
        for (auto error : *compilation.getError()) {
            std::cerr << error->format() << std::endl;
        }
    }

    if (compilation.isFailure()) {
        std::cerr << std::endl
                  << std::to_string(compilation.getError()->size()) +
                         " errors generated."
                  << std::endl;
    }
}
