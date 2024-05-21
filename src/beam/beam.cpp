#include "../../include/beam/compiler/compiler.hpp"
#include "../../include/beam/io/file/source.hpp"

#include <iostream>

int main(int argc, char** argv) {
    (void) argc;

    auto source = Beam::IO::File::Source(argv[0], argv[1]);
    auto compilation = Beam::Compiler::compile(source);

    std::cerr << source.getReader().debug() << std::endl;
}
