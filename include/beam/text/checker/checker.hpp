#pragma once

#include "../../diagnostic/diagnostic.hpp"
#include "../../io/format/types/map.hpp"
#include "../parser/syntax/ast.hpp"
#include "components/environment/environment.hpp"
#include "components/environment/owner/root.hpp"

namespace Beam::Text::Checker {
class Checker: IO::Format::Display {
  public:
    IO::Format::Types::Map<Components::Environment::Environment*,
                           Diagnostic::Diagnostic*>
    check(Parser::Syntax::AbstractSyntaxTree* tree);

    Components::Environment::Environment* getRoot() const { return root; }

    std::string format() override, debug() override;

  private:
    Components::Environment::Environment* root =
        new Components::Environment::Environment(
            new Components::Environment::Owner::Root);
};
} // namespace Beam::Text::Checker
