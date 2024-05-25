#pragma once

#include "type.hpp"

namespace Beam::Text::Parser::Syntax::Anotation::Type {
class Name: public Type {
  public:
    explicit Name(IO::Format::Types::String* value)
        : Type(Type::Kind::Name), value(value) {}

    IO::Format::Types::String* getValue() const { return value; }

    std::string format() override, debug() override;

  private:
    IO::Format::Types::String* value;
};
} // namespace Beam::Text::Parser::Syntax::Anotation::Type
