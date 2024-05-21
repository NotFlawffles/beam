#pragma once

#include "../../diagnostic/result.hpp"
#include "../debug/displayable_char.hpp"
#include "../source.hpp"
#include "token.hpp"

namespace Beam::Text::Lexer {
class Lexer {
  public:
    Lexer(const Text::Source& source)
        : source(source),
          span("lexer", 0, 1, 1,
               source.getReader().getValue().readAll().length()),
          current(source.getReader().getValue().readAll()[*span.getIndex()]) {}

    Diagnostic::Result<Token> lexNext(), lexIdentifier(), lexNumber(),
        lexCharacter(), lexString(), lexPlus(), lexMinus(), lexAsterisk(),
        lexSlash(), lexModulo(), lexAmpersand(), lexPipe(), lexCaret(),
        lexTilde(), lexLessThan(), lexGreaterThan(), lexExclamation(),
        lexAssign(), lexBrace(), lexComma(), lexColon(), lexSemiColon(),
        lexUnhandled(), lexEndOfFile(),
        advanceWithResult(const Diagnostic::Result<Token>& result);

    Diagnostic::Result<Debug::DisplayableChar> lexEscapeSequence();

    char skipWhitespace();
    char advance();

  private:
    const Text::Source source;

  public:
    Span span;
    char current;
};
} // namespace Beam::Text::Lexer
