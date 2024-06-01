#pragma once

#include "../../diagnostic/diagnostic.hpp"
#include "../../diagnostic/result.hpp"
#include "../../io/format/types/vector.hpp"
#include "../parser/syntax/annotation/declaration/type/name.hpp"
#include "../parser/syntax/annotation/declaration/type/pointer.hpp"
#include "../parser/syntax/annotation/declaration/type/type.hpp"
#include "../parser/syntax/ast.hpp"
#include "../parser/syntax/primary/block.hpp"
#include "../parser/syntax/primary/expression.hpp"
#include "../parser/syntax/statement/conditional/conditional.hpp"
#include "../parser/syntax/statement/conditional/if.hpp"
#include "../parser/syntax/statement/declaration/declaration.hpp"
#include "../parser/syntax/statement/declaration/function/function.hpp"
#include "../parser/syntax/statement/declaration/function/parameter.hpp"
#include "../parser/syntax/statement/declaration/variable.hpp"
#include "../parser/syntax/statement/return.hpp"
#include "../parser/syntax/statement/statement.hpp"
#include "environment.hpp"
#include "expression/evaluator.hpp"

namespace Beam::Text::Checker {
class Checker {
  public:
    Diagnostic::MonoResult<IO::Format::Types::Vector<Diagnostic::Diagnostic*>*>
    check(Parser::Syntax::AbstractSyntaxTree* syntax),

        checkNext(Parser::Syntax::AbstractSyntaxTree* syntax,
                  Environment* environment),

        checkPrimary(Parser::Syntax::Primary::Primary* syntax,
                     Environment* environment),

        checkPrimaryBlock(Parser::Syntax::Primary::Block* syntax,
                          Environment* environment),

        checkPrimaryExpression(Parser::Syntax::Primary::Expression* syntax,
                               Environment* environment),

        checkStatement(Parser::Syntax::Statement::Statement* syntax,
                       Environment* environment),

        checkStatementConditional(
            Parser::Syntax::Statement::Conditional::Conditional* syntax,
            Environment* environment),

        checkStatementConditionalIf(
            Parser::Syntax::Statement::Conditional::If* syntax,
            Environment* environment),

        checkStatementDeclaration(
            Parser::Syntax::Statement::Declaration::Declaration* syntax,
            Environment* environment),

        checkStatementDeclarationVariable(
            Parser::Syntax::Statement::Declaration::Variable* syntax,
            Environment* environment),

        checkStatementDeclarationFunction(
            Parser::Syntax::Statement::Declaration::Function::Function* syntax,
            Environment* environment),

        checkStatementDeclarationFunctionParameter(
            Parser::Syntax::Statement::Declaration::Function::Parameter* syntax,
            Environment* environment),

        checkStatementReturn(Parser::Syntax::Statement::Return* syntax,
                             Environment* environment),

        checkAnnotation(Parser::Syntax::Annotation::Annotation* syntax,
                        Environment* environment),

        checkAnnotationDeclarationType(
            Parser::Syntax::Annotation::Declaration::Type::Type* syntax,
            Environment* environment),

        checkAnnotationDeclarationTypeName(
            Parser::Syntax::Annotation::Declaration::Type::Name* syntax,
            Environment* environment),

        checkAnnotationDeclarationTypePointer(
            Parser::Syntax::Annotation::Declaration::Type::Pointer* syntax,
            Environment* environment),

        checkAnnotationDeclarationFlags(
            Parser::Syntax::Annotation::Declaration::Flags* syntax,
            Environment* environment);

    bool isPrimitiveType(const std::string& name) const;

    Expression::Evaluator getEvaluator() const { return evaluator; }

  private:
    Expression::Evaluator evaluator;
};
} // namespace Beam::Text::Checker
