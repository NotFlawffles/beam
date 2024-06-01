#pragma once

#include "diagnostic.hpp"

namespace Beam::Diagnostic {
class Note: public Diagnostic {
  public:
    Note(const std::string& message, const IO::String::Span& span,
         const bool& showLine = false)
        : Diagnostic(Diagnostic::Type::Note, span, message),
          showLine(showLine) {}

    std::string format() override, debug() override;

  private:
    const bool showLine;
};
} // namespace Beam::Diagnostic
