#include "../../../../../include/beam/text/checker/expression/evaluator.hpp"
#include "../../../../../include/beam/diagnostic/error.hpp"
#include "../../../../../include/beam/diagnostic/note.hpp"
#include "../../../../../include/beam/diagnostic/warning.hpp"
#include "../../../../../include/beam/text/parser/expression/literal.hpp"
#include "../../../../../include/beam/text/parser/syntax/statement/declaration/variable.hpp"

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Expression::Literal*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Checker::Expression::Evaluator::evaluate(
    Beam::Text::Parser::Expression::Expression* expression,
    Checker::Environment* environment) const {
    switch (expression->getType()) {
        case Parser::Expression::Expression::Type::Binary:
            return evaluateBinary(
                dynamic_cast<Parser::Expression::Binary*>(expression),
                environment);

        case Parser::Expression::Expression::Type::Unary:
            return evaluateUnary(
                dynamic_cast<Parser::Expression::Unary*>(expression),
                environment);

        case Parser::Expression::Expression::Type::Literal:
            return evaluateLiteral(
                dynamic_cast<Parser::Expression::Literal*>(expression),
                environment);
    }
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Expression::Literal*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Checker::Expression::Evaluator::evaluateBinary(
    Beam::Text::Parser::Expression::Binary* expression,
    Checker::Environment* environment) const {
    auto diagnostics = new Vec(Diagnostic::Diagnostic*, {});

    auto left = evaluate(expression->getLeft(), environment),
         right = evaluate(expression->getRight(), environment);

    if (left.isFailure()) {
        diagnostics->insert(diagnostics->end(), left.getError()->begin(),
                            left.getError()->end());
    }

    if (right.isFailure()) {
        diagnostics->insert(diagnostics->end(), right.getError()->begin(),
                            right.getError()->end());
    }

    if (left.getValue()->getLiteralType() !=
        right.getValue()->getLiteralType()) {
        diagnostics->push_back(new Diagnostic::Error(
            Diagnostic::Error::Type::ErrorTypeMismatchedTypes,
            Diagnostic::Error::Icon::ErrorIconProgramCross,
            right.getValue()->getSpan(), "mismatched types."));
    }

    if (left.getValue()->getLiteralType() ==
            Parser::Expression::Literal::Type::String ||
        right.getValue()->getLiteralType() ==
            Parser::Expression::Literal::Type::String) {
        diagnostics->push_back(new Diagnostic::Error(
            Diagnostic::Error::Type::ErrorTypeMismatchedTypes,
            Diagnostic::Error::Icon::ErrorIconProgramCross,
            right.getValue()->getSpan(),
            "string manipulation is not implemented yet."));
    }

    switch (expression->getOperator()->getType()) {
        case Parser::Expression::Operator::Type::Plus:
            switch (left.getValue()->getLiteralType()) {
                case Beam::Text::Parser::Expression::Literal::Type::Name:
                    // TODO
                    break;

                case Beam::Text::Parser::Expression::Literal::Type::Integer:
                    return Parser::Expression::Literal::Integer(
                        left.getValue()->getIntegerValue() +
                            right.getValue()->getIntegerValue(),
                        expression->getOperatorSpan());

                case Beam::Text::Parser::Expression::Literal::Type::Float:
                    return Parser::Expression::Literal::Float(
                        left.getValue()->getFloatValue() +
                            right.getValue()->getFloatValue(),
                        expression->getOperatorSpan());

                case Beam::Text::Parser::Expression::Literal::Type::Character:
                    return Parser::Expression::Literal::Character(
                        left.getValue()->getCharacterValue() +
                            right.getValue()->getCharacterValue(),
                        expression->getOperatorSpan());

                default:
                    break;
            }

        case Parser::Expression::Operator::Type::Minus:
            switch (left.getValue()->getLiteralType()) {
                case Beam::Text::Parser::Expression::Literal::Type::Name:
                    // TODO
                    break;

                case Beam::Text::Parser::Expression::Literal::Type::Integer:
                    return Parser::Expression::Literal::Integer(
                        left.getValue()->getIntegerValue() -
                            right.getValue()->getIntegerValue(),
                        expression->getOperatorSpan());

                case Beam::Text::Parser::Expression::Literal::Type::Float:
                    return Parser::Expression::Literal::Float(
                        left.getValue()->getFloatValue() -
                            right.getValue()->getFloatValue(),
                        expression->getOperatorSpan());

                case Beam::Text::Parser::Expression::Literal::Type::Character:
                    return Parser::Expression::Literal::Character(
                        left.getValue()->getCharacterValue() -
                            right.getValue()->getCharacterValue(),
                        expression->getOperatorSpan());

                default:
                    break;
            }

        case Parser::Expression::Operator::Type::Multiply:
            switch (left.getValue()->getLiteralType()) {
                case Beam::Text::Parser::Expression::Literal::Type::Name:
                    // TODO
                    break;

                case Beam::Text::Parser::Expression::Literal::Type::Integer:
                    return Parser::Expression::Literal::Integer(
                        left.getValue()->getIntegerValue() *
                            right.getValue()->getIntegerValue(),
                        expression->getOperatorSpan());

                case Beam::Text::Parser::Expression::Literal::Type::Float:
                    return Parser::Expression::Literal::Float(
                        left.getValue()->getFloatValue() *
                            right.getValue()->getFloatValue(),
                        expression->getOperatorSpan());

                case Beam::Text::Parser::Expression::Literal::Type::Character:
                    return Parser::Expression::Literal::Character(
                        left.getValue()->getCharacterValue() *
                            right.getValue()->getCharacterValue(),
                        expression->getOperatorSpan());

                default:
                    break;
            }

        case Parser::Expression::Operator::Type::Divide:
            switch (left.getValue()->getLiteralType()) {
                case Beam::Text::Parser::Expression::Literal::Type::Name:
                    // TODO
                    break;

                case Beam::Text::Parser::Expression::Literal::Type::Integer:
                    return Parser::Expression::Literal::Integer(
                        left.getValue()->getIntegerValue() /
                            right.getValue()->getIntegerValue(),
                        expression->getOperatorSpan());

                case Beam::Text::Parser::Expression::Literal::Type::Float:
                    return Parser::Expression::Literal::Float(
                        left.getValue()->getFloatValue() /
                            right.getValue()->getFloatValue(),
                        expression->getOperatorSpan());

                case Beam::Text::Parser::Expression::Literal::Type::Character:
                    return Parser::Expression::Literal::Character(
                        left.getValue()->getCharacterValue() /
                            right.getValue()->getCharacterValue(),
                        expression->getOperatorSpan());

                default:
                    break;
            }

        default:
            break;
    }

    return diagnostics;
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Expression::Literal*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Checker::Expression::Evaluator::evaluateUnary(
    Beam::Text::Parser::Expression::Unary* expression,
    Checker::Environment* environment) const {
    auto evaluated = evaluate(expression->getExpression(), environment);

    switch (expression->getOperator()->getType()) {
        case Parser::Expression::Operator::Type::Plus:
            switch (evaluated.getValue()->getLiteralType()) {
                case Parser::Expression::Literal::Type::Integer:
                case Parser::Expression::Literal::Type::Float:
                    return evaluated;

                default:
                    return new Vec(
                        Diagnostic::Diagnostic*,
                        {new Diagnostic::Error(
                            Diagnostic::Error::Type::ErrorTypeInvalidSyntax,
                            Diagnostic::Error::Icon::ErrorIconProgramCross,
                            expression->getOperatorSpan(),
                            "invalid unary expression")});
            }

            break;

        case Parser::Expression::Operator::Type::Minus:
            switch (evaluated.getValue()->getLiteralType()) {
                case Parser::Expression::Literal::Type::Integer:
                case Parser::Expression::Literal::Type::Float:
                    return evaluated;

                default:
                    return new Vec(
                        Diagnostic::Diagnostic*,
                        {new Diagnostic::Error(
                            Diagnostic::Error::Type::ErrorTypeInvalidSyntax,
                            Diagnostic::Error::Icon::ErrorIconProgramCross,
                            expression->getOperatorSpan(),
                            "invalid unary expression")});
            }

            break;

        case Parser::Expression::Operator::Type::Multiply:
            if (dynamic_cast<Parser::Expression::Literal*>(
                    expression->getExpression())
                    ->getLiteralType() !=
                Parser::Expression::Literal::Type::Name) {
                return new Vec(
                    Diagnostic::Diagnostic*,
                    {new Diagnostic::Error(
                        Diagnostic::Error::Type::ErrorTypeInvalidSyntax,
                        Diagnostic::Error::Icon::ErrorIconProgramCross,
                        expression->getOperatorSpan(),
                        "invalid unary expression")});
            }

            break;

        case Parser::Expression::Operator::Type::LogicalNot:
            break;

        case Parser::Expression::Operator::Type::BitwiseNot:
            break;
    }
}

Beam::Diagnostic::DiResult<
    Beam::Text::Parser::Expression::Literal*,
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Checker::Expression::Evaluator::evaluateLiteral(
    Beam::Text::Parser::Expression::Literal* expression,
    Checker::Environment* environment) const {
    if (expression->getLiteralType() ==
        Parser::Expression::Literal::Type::Name) {
        auto declaration = environment->get(expression->getNameValue());

        if (!declaration) {
            return new Vec(
                Diagnostic::Diagnostic*,
                {new Diagnostic::Warning(
                    Diagnostic::Warning::Type::WarningTypeUndeclaredName,
                    Diagnostic::Warning::Icon::WarningIconName,
                    expression->getSpan(),
                    "using undeclared name `" + expression->getNameValue() +
                        "`.")});
        }

        switch (declaration->getDeclarationType()) {
            case Beam::Text::Parser::Syntax::Statement::Declaration::
                Declaration::Type::Variable:
                return evaluate(
                    dynamic_cast<
                        Parser::Syntax::Statement::Declaration::Variable*>(
                        declaration)
                        ->getValue()
                        ->getValue(),
                    environment);

            case Beam::Text::Parser::Syntax::Statement::Declaration::
                Declaration::Type::Function:
                return new Vec(
                    Diagnostic::Diagnostic*,
                    {new Diagnostic::Note(
                        declaration->getName() +
                            "is a function, did you mean to call it?",
                        declaration->getName().getSpan())});

            case Beam::Text::Parser::Syntax::Statement::Declaration::
                Declaration::Type::FunctionParameter:
                return evaluate(
                    dynamic_cast<
                        Parser::Syntax::Statement::Declaration::Variable*>(
                        declaration)
                        ->getValue()
                        ->getValue(),
                    environment); // unsure
        }
    }

    return expression;
}
