#pragma once

#include "../../../io/format/display.hpp"
#include "../../../io/string/span.hpp"

namespace Beam::Text::Parser::Expression {
class Expression: public IO::Format::Display {
  public:
    enum class Type { Binary, Unary, Literal };

    explicit Expression(const Type& type): type(type) {}

    Type getType() const { return type; }

    std::string getTypeAsString() const;

  private:
    const Type type;
};
} // namespace Beam::Text::Parser::Expression
