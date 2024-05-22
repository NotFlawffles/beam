#pragma once

#include "../../../io/format/display.hpp"

namespace Beam::Text::Parser::Expression {
class Expression: public IO::Format::Display {
  public:
    enum class Type { Binary, Unary, Literal };

    explicit Expression(const Type& type): type(type) {}

    std::string getTypeAsString() const;

  private:
    const Type type;
};
} // namespace Beam::Text::Parser::Expression
