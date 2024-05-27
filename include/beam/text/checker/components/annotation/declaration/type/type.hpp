#pragma once

#include "../../../../../parser/syntax/annotation/declaration/type/type.hpp"

namespace Beam::Text::Checker::Components::Annotation::Declaration::Type {
class Type: public IO::Format::Display {
  public:
    enum class Kind { Primitive, Defined };

    Type(const Kind& kind, const unsigned long long int& depth)
        : kind(kind), depth(depth) {}

    Kind getKind() const { return kind; }

    unsigned long long int getDepth() const { return depth; }

    std::string getDepthAsString() const;

  private:
    const Kind kind;
    const unsigned long long int depth;
};
} // namespace Beam::Text::Checker::Components::Annotation::Declaration::Type
