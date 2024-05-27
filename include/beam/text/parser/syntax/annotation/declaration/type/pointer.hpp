#pragma once

#include "type.hpp"

namespace Beam::Text::Parser::Syntax::Annotation::Declaration::Type {
class Pointer: public Type {
  public:
    explicit Pointer(Type* type): Type(Type::Kind::Pointer), typeType(type) {}

    Type* getTypeType() const { return typeType; }

    IO::Format::Types::String* getTypeName() const;

    unsigned long long int getDepth() const;

    std::string format() override, debug() override;

  private:
    Type* typeType;
};
} // namespace Beam::Text::Parser::Syntax::Annotation::Declaration::Type
