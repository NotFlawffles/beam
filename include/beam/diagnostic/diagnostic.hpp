#pragma once

#include "../io/format/display.hpp"
#include "../io/string/span.hpp"

#define AppendIfFailure(diagnostics, target)                                   \
    if (target.isFailure())                                                    \
    diagnostics->push_back(target.getError())

#define InsertIfFailure(diagnostics, target)                                   \
    if (target.isFailure())                                                    \
    diagnostics->insert(diagnostics->end(), target.getError()->begin(),        \
                        target.getError()->end())

namespace Beam::Diagnostic {
class Diagnostic: public IO::Format::Display {
  public:
    enum class Type { Error, Warning, Note };

    Diagnostic(const Type& type, const IO::String::Span span,
               const std::string& message)
        : type(type), span(span), message(message) {}

    explicit Diagnostic(const Type& type): type(type) {}

    Type getType() const { return type; }

    IO::String::Span getSpan() const { return span; }

    std::string getMessage() const { return message; }

  private:
    const Type type;
    const IO::String::Span span;
    const std::string message;
};
} // namespace Beam::Diagnostic
