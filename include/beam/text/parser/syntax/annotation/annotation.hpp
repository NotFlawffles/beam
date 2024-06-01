#pragma once

#include "../../../../io/format/display.hpp"
#include "../ast.hpp"

namespace Beam::Text::Parser::Syntax::Annotation {
class Annotation: public AbstractSyntaxTree {
  public:
    enum class Type { Type, Flags };

    explicit Annotation(const Type& type)
        : AbstractSyntaxTree(AbstractSyntaxTree::Type::Anotation),
          annotationType(type) {}

    Type getAnnotationType() const { return annotationType; }

  private:
    const Type annotationType;
};
} // namespace Beam::Text::Parser::Syntax::Annotation
