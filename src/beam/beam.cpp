#include "../../include/beam/compiler/compiler.hpp"
#include "../../include/beam/io/file/source.hpp"

#include <iostream>

int main(int argc, char** argv) {
    (void) argc;

    auto source = Beam::IO::File::Source(argv[0], argv[1]);
    auto compilation = Beam::Compiler::compile(source);
    std::size_t errorCount = 0;

    if (compilation.isSuccess()) {
        for (auto error : compilation.getValue()) {
            std::cerr << error.format() << std::endl;
            errorCount++;
        }
    } else {
        std::cerr << compilation.getError().format() << std::endl;
        errorCount++;
    }

    if (errorCount) {
        std::cerr << std::endl
                  << std::to_string(errorCount) + " errors generated."
                  << std::endl;
    }
}
