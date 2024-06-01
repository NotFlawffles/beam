#pragma once

#include "../../../../../../io/string/span.hpp"
#include "type.hpp"

namespace Beam::Text::Parser::Syntax::Annotation::Declaration::Type {
class Name: public Type {
  public:
    explicit Name(IO::Format::Types::String* value,
                  const IO::String::Span& span)
        : Type(Type::Kind::Name), value(value), span(span) {}

    IO::Format::Types::String* getValue() const { return value; }

    IO::String::Span getSpan() const { return span; }

    std::string format() override, debug() override;

  private:
    IO::Format::Types::String* value;
    const IO::String::Span span;
};
} // namespace Beam::Text::Parser::Syntax::Annotation::Declaration::Type
