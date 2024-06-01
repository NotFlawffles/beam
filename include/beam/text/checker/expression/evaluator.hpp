#pragma once

#include "../../../diagnostic/diagnostic.hpp"
#include "../../../diagnostic/result.hpp"
#include "../../../io/format/display.hpp"
#include "../../../io/format/types/vector.hpp"
#include "../../parser/expression/binary.hpp"
#include "../../parser/expression/literal.hpp"
#include "../../parser/expression/unary.hpp"
#include "../environment.hpp"

namespace Beam::Text::Checker::Expression {
class Evaluator {
  public:
    Diagnostic::DiResult<Parser::Expression::Literal*,
                         IO::Format::Types::Vector<Diagnostic::Diagnostic*>*>
    evaluate(Parser::Expression::Expression* expression,
             Checker::Environment* environment) const,

        evaluateBinary(Parser::Expression::Binary* expression,
                       Checker::Environment* environment) const,

        evaluateUnary(Parser::Expression::Unary* expression,
                      Checker::Environment* environment) const,

        evaluateLiteral(Parser::Expression::Literal* expression,
                        Checker::Environment* environment) const;
};
} // namespace Beam::Text::Checker::Expression
