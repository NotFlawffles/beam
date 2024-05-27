#include "../../../../../../../../include/beam/text/checker/components/annotation/declaration/type/primitive.hpp"

std::string Beam::Text::Checker::Components::Annotation::Declaration::Type::
    Primitive::getPrimitiveTypeAsString() const {
    switch (getPrimitiveType()) {
        case Type::Uint8:
            return "uint8";

        case Type::Uint16:
            return "uint16";

        case Type::Uint32:
            return "uint32";

        case Type::Uint64:
            return "uint64";

        case Type::Sint8:
            return "sint8";

        case Type::Sint16:
            return "sint16";

        case Type::Sint32:
            return "sint32";

        case Type::Sint64:
            return "sint64";

        case Type::Char:
            return "char";

        default:
            return "unreachable";
    }
}

std::string Beam::Text::Checker::Components::Annotation::Declaration::Type::
    Primitive::format() {
    return getPrimitiveTypeAsString() + getDepthAsString();
}

std::string Beam::Text::Checker::Components::Annotation::Declaration::Type::
    Primitive::debug() {
    return "Primitive(primitiveType: " + getPrimitiveTypeAsString() +
           ", depth: " + std::to_string(getDepth()) + ')';
}
