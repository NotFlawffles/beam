#include "../../../../../../../../include/beam/text/parser/syntax/annotation/declaration/type/pointer.hpp"
#include "../../../../../../../../include/beam/compiler/utils/typechecking.hpp"
#include "../../../../../../../../include/beam/text/parser/syntax/annotation/declaration/type/name.hpp"

Beam::IO::Format::Types::String* Beam::Text::Parser::Syntax::Annotation::
    Declaration::Type::Pointer::getTypeName() const {
    auto type =
        dynamic_cast<Annotation::Declaration::Type::Type*>(new Pointer(*this));

    while (Compiler::Utils::isDynamicallyCastableTo<
           typename Annotation::Declaration::Type::Type*, Pointer*>(type)) {
        type = dynamic_cast<Pointer*>(type)->getTypeType();
    }

    return dynamic_cast<Annotation::Declaration::Type::Name*>(type)->getValue();
}

unsigned long long int
Beam::Text::Parser::Syntax::Annotation::Declaration::Type::Pointer::getDepth()
    const {
    auto depth = 0;

    auto type =
        dynamic_cast<Annotation::Declaration::Type::Type*>(new Pointer(*this));

    while (Compiler::Utils::isDynamicallyCastableTo<
           typename Annotation::Declaration::Type::Type*, Pointer*>(type)) {
        type = dynamic_cast<Pointer*>(type)->getTypeType();
        depth++;
    }

    return depth;
}

std::string
Beam::Text::Parser::Syntax::Annotation::Declaration::Type::Pointer::format() {
    auto result = std::string(getTypeName()->format());

    for (unsigned long long int index = 0; index < getDepth(); index++) {
        result.push_back('*');
    }

    return result;
}

std::string
Beam::Text::Parser::Syntax::Annotation::Declaration::Type::Pointer::debug() {
    return "Pointer(typeType: " + getTypeType()->debug() +
           ", depth: " + std::to_string(getDepth()) + ')';
}
