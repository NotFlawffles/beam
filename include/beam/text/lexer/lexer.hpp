#pragma once

#include "../../diagnostic/result.hpp"
#include "../../io/file/source.hpp"
#include "../../io/format/types/char.hpp"
#include "token.hpp"

namespace Beam::Text::Lexer {
class Lexer {
  public:
    Lexer(const IO::File::Source& source)
        : source(source),
          span("lexer", source.getPath().string() + ':', 0, 1, 1,
               source.getReader().getValue()->readAll().length()),
          current(source.getReader().getValue()->readAll()[*span.getIndex()]) {}

    Diagnostic::DiResult<Token*, Diagnostic::Diagnostic*> lexNext(),
        lexIdentifier(), lexNumber(), lexCharacter(), lexString(), lexPlus(),
        lexMinus(), lexAsterisk(), lexSlash(), lexModulo(), lexAmpersand(),
        lexPipe(), lexCaret(), lexTilde(), lexLessThan(), lexGreaterThan(),
        lexExclamation(), lexAssign(), lexBrace(), lexComma(), lexColon(),
        lexSemiColon(), lexUnhandled(), lexEndOfFile(),
        advanceWithResult(
            const Diagnostic::DiResult<Token*, Diagnostic::Diagnostic*>
                & result);

    Diagnostic::DiResult<IO::Format::Types::Char*, Diagnostic::Diagnostic*>
    lexEscapeSequence();

    char skipWhitespace();
    char advance();

  private:
    const IO::File::Source source;

  public:
    IO::String::Span span;
    char current;
};
} // namespace Beam::Text::Lexer
