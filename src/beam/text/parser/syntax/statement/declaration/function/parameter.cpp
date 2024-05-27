#include "../../../../../../../../include/beam/text/parser/syntax/statement/declaration/function/parameter.hpp"

std::string Beam::Text::Parser::Syntax::Statement::Declaration::Function::
    Parameter::format() {
    return getFlags()->format() + ' ' + getName() + ": " +
           getAnotationType()->format();
}

std::string Beam::Text::Parser::Syntax::Statement::Declaration::Function::
    Parameter::debug() {
    return "Parameter(type: " + getAnotationType()->debug() +
           ", flags: " + getFlags()->debug() + ", name: " + getName() + ')';
}
