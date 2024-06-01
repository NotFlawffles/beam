#include <algorithm>

#include "../../../../include/beam/diagnostic/error.hpp"
#include "../../../../include/beam/diagnostic/note.hpp"
#include "../../../../include/beam/diagnostic/warning.hpp"
#include "../../../../include/beam/io/file/reader.hpp"
#include "../../../../include/beam/text/checker/checker.hpp"

Beam::Diagnostic::MonoResult<
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Checker::Checker::check(
    Beam::Text::Parser::Syntax::AbstractSyntaxTree* syntax) {
    return checkPrimaryBlock(
        dynamic_cast<Parser::Syntax::Primary::Block*>(syntax),
        new Environment(nullptr));
}

Beam::Diagnostic::MonoResult<
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Checker::Checker::checkNext(
    Beam::Text::Parser::Syntax::AbstractSyntaxTree* syntax,
    Beam::Text::Checker::Environment* environment) {
    switch (syntax->getType()) {
        case Beam::Text::Parser::Syntax::AbstractSyntaxTree::Type::Primary:
            return checkPrimary(
                dynamic_cast<Parser::Syntax::Primary::Primary*>(syntax),
                environment);

        case Beam::Text::Parser::Syntax::AbstractSyntaxTree::Type::Statement:
            return checkStatement(
                dynamic_cast<Parser::Syntax::Statement::Statement*>(syntax),
                environment);

        case Beam::Text::Parser::Syntax::AbstractSyntaxTree::Type::Anotation:
            return checkAnnotation(
                dynamic_cast<Parser::Syntax::Annotation::Annotation*>(syntax),
                environment);
    }
}

Beam::Diagnostic::MonoResult<
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Checker::Checker::checkPrimary(
    Beam::Text::Parser::Syntax::Primary::Primary* syntax,
    Beam::Text::Checker::Environment* environment) {
    switch (syntax->getPrimaryType()) {
        case Beam::Text::Parser::Syntax::Primary::Primary::Type::Block:
            return checkPrimaryBlock(
                dynamic_cast<Parser::Syntax::Primary::Block*>(syntax),
                environment);

        case Beam::Text::Parser::Syntax::Primary::Primary::Type::Expression:
            return checkPrimaryExpression(
                dynamic_cast<Parser::Syntax::Primary::Expression*>(syntax),
                environment);
    }
}

Beam::Diagnostic::MonoResult<
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Checker::Checker::checkPrimaryBlock(
    Beam::Text::Parser::Syntax::Primary::Block* syntax,
    Beam::Text::Checker::Environment* environment) {
    auto diagnostics = new Vec(Diagnostic::Diagnostic*, {});

    for (const auto& branch : syntax->getValue()) {
        auto next = checkNext(branch, environment);
        InsertIfFailure(diagnostics, next);
    }

    return diagnostics;
}

Beam::Diagnostic::MonoResult<
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Checker::Checker::checkPrimaryExpression(
    Beam::Text::Parser::Syntax::Primary::Expression* syntax,
    Beam::Text::Checker::Environment* environment) {
    auto evaluation = evaluator.evaluate(syntax->getValue(), environment);

    if (evaluation.isFailure()) {
        return evaluation.getError();
    }

    return {};
}

Beam::Diagnostic::MonoResult<
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Checker::Checker::checkStatement(
    Beam::Text::Parser::Syntax::Statement::Statement* syntax,
    Beam::Text::Checker::Environment* environment) {
    switch (syntax->getStatementType()) {
        case Beam::Text::Parser::Syntax::Statement::Statement::Type::
            Conditional:
            return checkStatementConditional(
                dynamic_cast<
                    Parser::Syntax::Statement::Conditional::Conditional*>(
                    syntax),
                environment);

        case Beam::Text::Parser::Syntax::Statement::Statement::Type::
            Declaration:
            return checkStatementDeclaration(
                dynamic_cast<
                    Parser::Syntax::Statement::Declaration::Declaration*>(
                    syntax),
                environment);

        case Beam::Text::Parser::Syntax::Statement::Statement::Type::Return:
            return checkStatementReturn(
                dynamic_cast<Parser::Syntax::Statement::Return*>(syntax),
                environment);
    }
}

Beam::Diagnostic::MonoResult<
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Checker::Checker::checkStatementConditional(
    Beam::Text::Parser::Syntax::Statement::Conditional::Conditional* syntax,
    Beam::Text::Checker::Environment* environment) {
    if (syntax->getConditionalType() ==
        Parser::Syntax::Statement::Conditional::Conditional::Type::If) {
        return checkStatementConditionalIf(
            dynamic_cast<Parser::Syntax::Statement::Conditional::If*>(syntax),
            environment);
    }

    auto diagnostics = new Vec(Diagnostic::Diagnostic*, {});

    auto conditionCheck =
             checkPrimaryExpression(syntax->getCondition(), environment),

         bodyCheck =
             checkPrimaryBlock(syntax->getBody(), new Environment(environment));

    InsertIfFailure(diagnostics, conditionCheck);
    InsertIfFailure(diagnostics, bodyCheck);

    if (!diagnostics->empty()) {
        return diagnostics;
    }

    return {};
}

Beam::Diagnostic::MonoResult<
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Checker::Checker::checkStatementConditionalIf(
    Beam::Text::Parser::Syntax::Statement::Conditional::If* syntax,
    Beam::Text::Checker::Environment* environment) {
    auto diagnostics = new Vec(Diagnostic::Diagnostic*, {});

    auto scope = new Environment(environment);

    auto conditionCheck = checkPrimaryExpression(syntax->getCondition(), scope),
         bodyCheck = checkPrimaryBlock(syntax->getBody(), scope);

    InsertIfFailure(diagnostics, conditionCheck);
    InsertIfFailure(diagnostics, bodyCheck);

    auto currentElseClause = syntax->getElseClause();

    while (currentElseClause) {
        auto currentElseClauseScope = new Environment(environment);

        if (currentElseClause->getCondition()) {
            auto currentElseClauseConditionCheck = checkPrimaryExpression(
                currentElseClause->getCondition(), scope);
            InsertIfFailure(diagnostics, currentElseClauseConditionCheck);
        }

        auto currentElseClauseBodyCheck = checkPrimaryBlock(
            currentElseClause->getBody(), currentElseClauseScope);

        InsertIfFailure(diagnostics, currentElseClauseBodyCheck);

        currentElseClause = currentElseClause->getElseClause();
    }

    if (!diagnostics->empty()) {
        return diagnostics;
    }

    return {};
}

Beam::Diagnostic::MonoResult<
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Checker::Checker::checkStatementDeclaration(
    Beam::Text::Parser::Syntax::Statement::Declaration::Declaration* syntax,
    Beam::Text::Checker::Environment* environment) {
    auto diagnostics = new Vec(Diagnostic::Diagnostic*, {});

    if (environment->exists(syntax->getName(), false)) {
        diagnostics->push_back(new Diagnostic::Error(
            Diagnostic::Error::Type::ErrorTypeRedefinition,
            Diagnostic::Error::Icon::ErrorIconName, syntax->getName().getSpan(),
            "redefinition of `" + syntax->getName() + "`."));

        auto previousDefinition = environment->get(syntax->getName());

        diagnostics->push_back(new Diagnostic::Note(
            "initial definition occured here:",
            previousDefinition->getName().getSpan(), true));
    } else if (isPrimitiveType(syntax->getName().get())) {
        diagnostics->push_back(new Diagnostic::Error(
            Diagnostic::Error::Type::ErrorTypeInvalidSyntax,
            Diagnostic::Error::Icon::ErrorIconProgramCross,
            syntax->getName().getSpan(),
            "using primitive type name `" + syntax->getName() +
                "` as a name."));
    }

    auto annotationFlagsCheck =
             checkAnnotationDeclarationFlags(syntax->getFlags(), environment),

         annotationTypeCheck = checkAnnotationDeclarationType(
             syntax->getAnotationType(), environment);

    InsertIfFailure(diagnostics, annotationFlagsCheck);
    InsertIfFailure(diagnostics, annotationTypeCheck);

    Diagnostic::MonoResult<IO::Format::Types::Vector<Diagnostic::Diagnostic*>*>
        check;

    switch (syntax->getDeclarationType()) {
        case Beam::Text::Parser::Syntax::Statement::Declaration::Declaration::
            Type::Variable:
            check = checkStatementDeclarationVariable(
                dynamic_cast<Parser::Syntax::Statement::Declaration::Variable*>(
                    syntax),
                environment);
            break;

        case Beam::Text::Parser::Syntax::Statement::Declaration::Declaration::
            Type::Function:
            check = checkStatementDeclarationFunction(
                dynamic_cast<Parser::Syntax::Statement::Declaration::Function::
                                 Function*>(syntax),
                environment);
            break;

        case Beam::Text::Parser::Syntax::Statement::Declaration::Declaration::
            Type::FunctionParameter:
            checkStatementDeclarationFunctionParameter(
                dynamic_cast<Parser::Syntax::Statement::Declaration::Function::
                                 Parameter*>(syntax),
                environment);
            break;
    }

    InsertIfFailure(diagnostics, check);

    if (!diagnostics->empty()) {
        return diagnostics;
    }

    return {};
}

Beam::Diagnostic::MonoResult<
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Checker::Checker::checkStatementDeclarationVariable(
    Beam::Text::Parser::Syntax::Statement::Declaration::Variable* syntax,
    Beam::Text::Checker::Environment* environment) {
    auto diagnostics = new Vec(Diagnostic::Diagnostic*, {});

    auto valueCheck = checkPrimaryExpression(syntax->getValue(), environment);

    InsertIfFailure(diagnostics, valueCheck);

    if (!diagnostics->empty()) {
        return diagnostics;
    }

    environment->add(syntax);
    return {};
}

Beam::Diagnostic::MonoResult<
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Checker::Checker::checkStatementDeclarationFunction(
    Beam::Text::Parser::Syntax::Statement::Declaration::Function::Function*
        syntax,
    Beam::Text::Checker::Environment* environment) {
    auto diagnostics = new Vec(Diagnostic::Diagnostic*, {});

    auto parametersScope = new Environment(environment);

    for (const auto& parameter : *syntax->getParameters()) {
        auto check = checkStatementDeclaration(parameter, parametersScope);
        InsertIfFailure(diagnostics, check);
    }

    auto bodyCheck =
        checkPrimaryBlock(syntax->getBody(), new Environment(parametersScope));

    InsertIfFailure(diagnostics, bodyCheck);

    if (!diagnostics->empty()) {
        return diagnostics;
    }

    environment->add(syntax);
    return {};
}

Beam::Diagnostic::MonoResult<
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Checker::Checker::checkStatementDeclarationFunctionParameter(
    Beam::Text::Parser::Syntax::Statement::Declaration::Function::Parameter*
        syntax,
    Beam::Text::Checker::Environment* environment) {
    environment->add(syntax);
    return {};
}

Beam::Diagnostic::MonoResult<
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Checker::Checker::checkAnnotation(
    Beam::Text::Parser::Syntax::Annotation::Annotation* syntax,
    Beam::Text::Checker::Environment* environment) {
    switch (syntax->getAnnotationType()) {
        case Beam::Text::Parser::Syntax::Annotation::Annotation::Type::Type:
            return checkAnnotationDeclarationType(
                dynamic_cast<
                    Parser::Syntax::Annotation::Declaration::Type::Type*>(
                    syntax),
                environment);

        case Beam::Text::Parser::Syntax::Annotation::Annotation::Type::Flags:
            return checkAnnotationDeclarationFlags(
                dynamic_cast<Parser::Syntax::Annotation::Declaration::Flags*>(
                    syntax),
                environment);
    }
}

Beam::Diagnostic::MonoResult<
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Checker::Checker::checkAnnotationDeclarationType(
    Beam::Text::Parser::Syntax::Annotation::Declaration::Type::Type* syntax,
    Beam::Text::Checker::Environment* environment) {
    switch (syntax->getKind()) {
        case Beam::Text::Parser::Syntax::Annotation::Declaration::Type::Type::
            Kind::Name:
            return checkAnnotationDeclarationTypeName(
                dynamic_cast<
                    Parser::Syntax::Annotation::Declaration::Type::Name*>(
                    syntax),
                environment);

        case Beam::Text::Parser::Syntax::Annotation::Declaration::Type::Type::
            Kind::Pointer:
            return checkAnnotationDeclarationTypePointer(
                dynamic_cast<
                    Parser::Syntax::Annotation::Declaration::Type::Pointer*>(
                    syntax),
                environment);
    }
}

Beam::Diagnostic::MonoResult<
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Checker::Checker::checkAnnotationDeclarationTypePointer(
    Beam::Text::Parser::Syntax::Annotation::Declaration::Type::Pointer* syntax,
    Beam::Text::Checker::Environment* environment) {
    Parser::Syntax::Annotation::Declaration::Type::Type* current = syntax;

    while (current->getKind() !=
           Parser::Syntax::Annotation::Declaration::Type::Type::Kind::Name) {
        current = dynamic_cast<
                      Parser::Syntax::Annotation::Declaration::Type::Pointer*>(
                      current)
                      ->getTypeType();
    }

    return checkAnnotationDeclarationTypeName(
        dynamic_cast<Parser::Syntax::Annotation::Declaration::Type::Name*>(
            current),
        environment);
}

Beam::Diagnostic::MonoResult<
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Checker::Checker::checkAnnotationDeclarationTypeName(
    Beam::Text::Parser::Syntax::Annotation::Declaration::Type::Name* syntax,
    Beam::Text::Checker::Environment* environment) {
    (void) environment;

    auto diagnostics = new Vec(Diagnostic::Diagnostic*, {});

    if (!isPrimitiveType(syntax->getValue()->get())) {
        diagnostics->push_back(new Diagnostic::Warning(
            Diagnostic::Warning::Type::WarningTypeUnknownType,
            Diagnostic::Warning::Icon::WarningIconRectangle, syntax->getSpan(),
            "unknown type name `" + syntax->getValue()->get() + "`."));

        diagnostics->push_back(new Diagnostic::Note("defaulting to `i32`",
                                                    syntax->getSpan(), false));
    }

    if (!diagnostics->empty()) {
        return diagnostics;
    }

    return {};
}

Beam::Diagnostic::MonoResult<
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Checker::Checker::checkAnnotationDeclarationFlags(
    Beam::Text::Parser::Syntax::Annotation::Declaration::Flags* syntax,
    Beam::Text::Checker::Environment* environment) {
    (void) syntax;
    (void) environment;
    return {};
}

Beam::Diagnostic::MonoResult<
    Beam::IO::Format::Types::Vector<Beam::Diagnostic::Diagnostic*>*>
Beam::Text::Checker::Checker::checkStatementReturn(
    Beam::Text::Parser::Syntax::Statement::Return* syntax,
    Beam::Text::Checker::Environment* environment) {
    return checkPrimaryExpression(syntax->getExpression(), environment);
}

bool Beam::Text::Checker::Checker::isPrimitiveType(
    const std::string& name) const {
    std::vector<std::string> primitives = {"i8",  "i16", "i32", "i64", "u8",
                                           "u16", "u32", "u64", "char"};

    return std::find(primitives.begin(), primitives.end(), name) !=
           primitives.end();
}
