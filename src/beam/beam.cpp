#include "../../include/beam/compiler/compiler.hpp"
#include "../../include/beam/io/file/source.hpp"

#include "../../include/beam/io/format/color/colorscheme.hpp"
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
            std::cerr << (argv[2] ? value->debug() : value->format())
                      << std::endl;
        }
    } else {
        for (auto error : *compilation.getError()) {
            if (!error) {
                continue;
            }

            std::cerr << (argv[2] ? error->debug() : error->format())
                      << std::endl;
        }
    }

    if (compilation.isFailure()) {
        std::cerr << std::endl
                  << Beam::IO::Format::Color::Colorscheme(
                         Beam::IO::Format::Types::Map<
                             Beam::IO::Format::Types::String*,
                             Beam::IO::Format::Color::Color*>(
                             {{new Beam::IO::Format::Types::String("error"),
                               new Beam::IO::Format::Color::Color(
                                   Beam::IO::Format::Color::Color::Type::
                                       ColorTypeRed)}}))
                         .color(
                             "#{error}(" +
                             std::to_string(compilation.getError()->size()) +
                             " error" +
                             (compilation.getError()->size() > 1 ? "s " : " ") +
                             "generated.#)")
                         .format()

                  << std::endl;
    }
}
