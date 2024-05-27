#include "../../../../../../../../include/beam/text/checker/components/annotation/declaration/type/type.hpp"

std::string Beam::Text::Checker::Components::Annotation::Declaration::Type::
    Type::getDepthAsString() const {
    auto content = std::string();

    for (unsigned long long int index = 0; index < getDepth();
         index++, content.push_back('*'))
        ;

    return content;
}
