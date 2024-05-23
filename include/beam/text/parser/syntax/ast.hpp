#pragma once

#include "../../../io/format/display.hpp"

namespace Beam::Text::Parser::Syntax {
class AbstractSyntaxTree: IO::Format::Display {
  public:
    enum class Type { Primary, Statement };

    AbstractSyntaxTree(const Type& type): type(type) {}

    Type getType() const { return type; }

    std::string getTypeAsString() const;

    std::string format() { return "DefaultFormatter"; }

    std::string debug() { return "DefaultDebugger"; }

  private:
    const Type type;
};
} // namespace Beam::Text::Parser::Syntax
