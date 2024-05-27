#pragma once

#include "../../declaration/declaration.hpp"

namespace Beam::Text::Checker::Components::Environment::Owner {
class Owner: public IO::Format::Display {
  public:
    enum class Type { Root, Statement };

    Owner(const Type& type): ownerType(type) {}

    Type getOwnerType() const { return ownerType; }

  private:
    const Type ownerType;
};
} // namespace Beam::Text::Checker::Components::Environment::Owner
