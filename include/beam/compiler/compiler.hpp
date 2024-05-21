#pragma once

#include "../diagnostic/error.hpp"
#include "../diagnostic/result.hpp"
#include "../text/debug/displayable_vector.hpp"
#include "../text/source.hpp"

namespace Beam::Compiler {
Diagnostic::Result<Text::Debug::DisplayableVector<Diagnostic::Error>>
compile(const Text::Source& source);
}
