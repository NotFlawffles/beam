#include "../../../include/beam/compiler/compiler.hpp"
#include "../../../include/beam/text/source.hpp"

#include <iostream>

int main(int argc, char** argv) {
    (void) argc;

    auto source = Beam::Text::Source(argv[0], argv[1]);
    auto compilation = Beam::Compiler::compile(source);

    std::cerr << source.getReader().debug() << std::endl;
}
