#pragma once

namespace Beam::Compiler::Utils {
template<typename T, typename U> bool isDynamicallyCastableTo(const T& which) {
    return dynamic_cast<U>(which);
}
} // namespace Beam::Compiler::Utils
