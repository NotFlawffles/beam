#pragma once

#include "../../../../diagnostic/diagnostic.hpp"
#include "../../../../diagnostic/result.hpp"
#include "../../../../io/format/display.hpp"
#include "../../../../io/format/types/vector.hpp"
#include "../declaration/declaration.hpp"
#include "owner/owner.hpp"

namespace Beam::Text::Checker::Components::Environment {
class Environment: IO::Format::Display {
  public:
    Environment(Owner::Owner* owner)
        : owner(owner), declarations(new Vec(Declaration::Declaration*, {})) {}

    Diagnostic::DiResult<Declaration::Declaration*, Diagnostic::Diagnostic*>
    get(const std::string& name, const bool& lookForParents = true),
        get(Declaration::Declaration* declaration,
            const bool& lookForParents = true),
        add(Declaration::Declaration* declaration);

    Owner::Owner* getOwner() const { return owner; }

    Environment* getParent() const { return parent; }

    IO::String::Span* getSpan() const { return span; }

    IO::Format::Types::Vector<Declaration::Declaration*>*
    getDeclarations() const {
        return declarations;
    }

    std::string format() override, debug() override;

  private:
    Owner::Owner* owner;
    Environment* parent;
    IO::String::Span* span;
    IO::Format::Types::Vector<Declaration::Declaration*>* declarations;
};
} // namespace Beam::Text::Checker::Components::Environment
