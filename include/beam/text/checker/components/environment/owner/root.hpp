#pragma once

#include "owner.hpp"

namespace Beam::Text::Checker::Components::Environment::Owner {
class Root: public Owner {
  public:
    explicit Root(): Owner(Owner::Type::Root) {}

    std::string format() override, debug() override;
};
} // namespace Beam::Text::Checker::Components::Environment::Owner
