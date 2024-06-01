#pragma once

#include "../../../../../../io/format/types/string.hpp"
#include "../../annotation.hpp"

namespace Beam::Text::Parser::Syntax::Annotation::Declaration::Type {
class Type: public Annotation {
  public:
    enum class Kind { Name, Pointer };

    Type(const Kind& kind): Annotation(Annotation::Type::Type), kind(kind) {}

    Kind getKind() const { return kind; }

  private:
    const Kind kind;
};
} // namespace Beam::Text::Parser::Syntax::Annotation::Declaration::Type
