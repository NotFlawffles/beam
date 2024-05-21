#pragma once

#include "../debug/types/vector.hpp"
#include "../diagnostic/error.hpp"
#include "../diagnostic/result.hpp"
#include "../io/file/source.hpp"

namespace Beam::Compiler {
Diagnostic::Result<Debug::Types::Vector<Diagnostic::Error>>
compile(const IO::File::Source& source);
}
