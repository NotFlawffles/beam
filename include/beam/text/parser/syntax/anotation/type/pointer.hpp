#pragma once

#include "type.hpp"

namespace Beam::Text::Parser::Syntax::Anotation::Type {
class Pointer: public Type {
  public:
    explicit Pointer(Type* type): Type(Type::Kind::Pointer), type(type) {}

    Type* getType() const { return type; }

    std::string format() override, debug() override;

  private:
    Type* type;
};
} // namespace Beam::Text::Parser::Syntax::Anotation::Type
