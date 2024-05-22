#pragma once

#include "../diagnostic/error.hpp"
#include "../diagnostic/result.hpp"
#include "../io/file/source.hpp"
#include "../io/format/types/vector.hpp"

namespace Beam::Compiler {
Diagnostic::Result<IO::Format::Types::Vector<Diagnostic::Error*>*,
                   Diagnostic::Error*>
compile(const IO::File::Source& source, const bool& debug);
}
