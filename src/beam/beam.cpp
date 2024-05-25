#include "../../include/beam/compiler/compiler.hpp"
#include "../../include/beam/io/file/source.hpp"

#include "../../include/beam/io/format/colors.hpp"
#include "../../include/beam/text/parser/syntax/anotation/type/pointer.hpp"
#include "../../include/beam/text/parser/syntax/anotation/type/type.hpp"
#include "../../include/beam/text/parser/syntax/primary/block.hpp"
#include "../../include/beam/text/parser/syntax/statement/declaration/variable.hpp"

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
            std::cerr << error->format() << std::endl;
        }
    }

    if (compilation.isFailure()) {
        std::cerr << std::endl
                  << Beam::IO::Format::Colors::Colorscheme(
                         Beam::IO::Format::Types::Map<
                             Beam::IO::Format::Types::String*,
                             Beam::IO::Format::Colors::Color*>(
                             {{new Beam::IO::Format::Types::String("error"),
                               new Beam::IO::Format::Colors::Color(
                                   Beam::IO::Format::Colors::Color::Type::
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
