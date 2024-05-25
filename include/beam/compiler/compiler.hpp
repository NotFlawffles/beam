#pragma once

#include "../diagnostic/error.hpp"
#include "../diagnostic/result.hpp"
#include "../io/file/source.hpp"
#include "../io/format/types/vector.hpp"
#include "../text/parser/syntax/ast.hpp"

namespace Beam::Compiler {
Diagnostic::DiResult<Text::Parser::Syntax::AbstractSyntaxTree*,
                     IO::Format::Types::Vector<Diagnostic::Error*>*>
compile(const IO::File::Source& source);
}
