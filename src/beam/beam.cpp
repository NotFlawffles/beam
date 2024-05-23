#include "../../include/beam/compiler/compiler.hpp"
#include "../../include/beam/io/file/source.hpp"

#include "../../include/beam/text/parser/expression/literal.hpp"
#include "../../include/beam/text/parser/syntax/statement/conditional/if.hpp"

#include <iostream>

int main(int argc, char** argv) {
    (void) argc;

    //    auto source = Beam::IO::File::Source(argv[0], argv[1]);
    //    auto compilation = Beam::Compiler::compile(source, argv[2]);
    //    std::size_t errorCount = 0;

    auto expression = Beam::Text::Parser::Expression::Literal::Integer(69);

    auto firstCondition =
        Beam::Text::Parser::Syntax::Primary::Expression(expression);

    auto firstBlockVector = Beam::IO::Format::Types::Vector<
        Beam::Text::Parser::Syntax::AbstractSyntaxTree*>({&firstCondition});

    auto firstBlock =
        Beam::Text::Parser::Syntax::Primary::Block(firstBlockVector);

    auto elseClauseStatement =
        Beam::Text::Parser::Syntax::Statement::Conditional::If(
            nullptr, &firstBlock, nullptr);

    auto ifStatement = Beam::Text::Parser::Syntax::Statement::Conditional::If(
        &firstCondition, &firstBlock, &elseClauseStatement);

    std::cerr << ifStatement.debug() << std::endl;

    // if (compilation.isSuccess()) {
    //     for (auto error : *compilation.getValue()) {
    //         std::cerr << error->format() << std::endl;
    //         errorCount++;
    //     }
    // } else {
    //     std::cerr << compilation.getError()->format() << std::endl;
    //     errorCount++;
    // }

    // if (errorCount) {
    //     std::cerr << std::endl
    //               << std::to_string(errorCount) + " errors generated."
    //               << std::endl;
    // }
}
