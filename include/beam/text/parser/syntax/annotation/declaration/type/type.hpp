#pragma once

#include "../../../../../../io/format/types/string.hpp"
#include "../../../ast.hpp"

namespace Beam::Text::Parser::Syntax::Annotation::Declaration::Type {
class Type: public AbstractSyntaxTree {
  public:
    enum class Kind { Name, Pointer };

    Type(const Kind& kind)
        : AbstractSyntaxTree(AbstractSyntaxTree::Type::Anotation), kind(kind) {}

    Kind getKind() const { return kind; }

  private:
    const Kind kind;
};
} // namespace Beam::Text::Parser::Syntax::Annotation::Declaration::Type
