#pragma once

#include "../../../../../io/format/types/char.hpp"
#include "../annotation.hpp"

namespace Beam::Text::Parser::Syntax::Annotation::Declaration {
class Flags: public Annotation {
  public:
    enum Type { Public = 1, Private = 2, Mutable = 4, Immutable = 8 };

    explicit Flags(IO::Format::Types::Char* value)
        : Annotation(Annotation::Type::Flags), value(value) {}

    IO::Format::Types::Char* getValue() const { return value; }

    bool isPublic() const { return value->getValue() & Type::Public; }

    bool isPrivate() const { return value->getValue() & Type::Private; }

    bool isMutable() const { return value->getValue() & Type::Mutable; }

    bool isImmutable() const { return value->getValue() & Type::Immutable; }

    std::string format() override {
        return std::string(isPublic() ? "pub" : "priv") + ' ' +
               (isMutable() ? "mut" : "imut");
    }

    std::string debug() override {
        return "Flags(value: " + std::to_string((int) getValue()->getValue()) +
               ')';
    }

  private:
    IO::Format::Types::Char* value;
};
} // namespace Beam::Text::Parser::Syntax::Annotation::Declaration
