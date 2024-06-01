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
            std::cerr << (argv[2] ? error->debug() : error->format())
                      << std::endl
                      << std::endl;
        }
    }

    if (compilation.isFailure()) {
        auto colorscheme = Beam::IO::Format::Color::Colorscheme(
            Beam::IO::Format::Types::Map<Beam::IO::Format::Types::String*,
                                         Beam::IO::Format::Color::Color*>(
                {{new Beam::IO::Format::Types::String("error"),
                  new Beam::IO::Format::Color::Color(
                      Beam::IO::Format::Color::Color::Type::ColorTypeRed)},
                 {new Beam::IO::Format::Types::String("warning"),
                  new Beam::IO::Format::Color::Color(
                      Beam::IO::Format::Color::Color::Type::
                          ColorTypeYellow)}}));

        auto errors = 0, warnings = 0;

        for (const auto& diagnostic : *compilation.getError()) {
            if (diagnostic->getType() ==
                Beam::Diagnostic::Diagnostic::Type::Error) {
                errors++;
            } else if (diagnostic->getType() ==
                       Beam::Diagnostic::Diagnostic::Type::Warning) {
                warnings++;
            }
        }

        auto message = std::string();

        if (errors) {
            message.append("#{error}(" + std::to_string(errors) + " errors#)");
        }

        if (warnings) {
            if (errors) {
                message.append(", ");
            }

            message.append("#{warning}(" + std::to_string(warnings) +
                           " warnings#)");
        }

        message.append(" generated.");

        std::cerr << colorscheme.color(message).format() << std::endl;
    }
}
