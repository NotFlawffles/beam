#pragma once

#include "diagnostic.hpp"

namespace Beam::Diagnostic {
class Warning: public Diagnostic {
  public:
    enum Type : unsigned char { WarningTypeUndeclaredName, WarningTypeCount };

    enum Icon : unsigned char { WarningIconName, WarningIconCount };

    Warning(const Type& type, const Icon& icon, const IO::String::Span& span,
            const std::string& message)
        : Diagnostic(Diagnostic::Type::Warning, span, message),
          warningType(type), icon(icon) {}

    explicit Warning()
        : Diagnostic(Diagnostic::Type::Warning),
          warningType(Type::WarningTypeUndeclaredName),
          icon(Icon::WarningIconName) {}

    unsigned char getNumber() const { return warningType; }

    std::string getTypeAsString() const, getIconAsString() const,
        format() override, debug() override;

  private:
    const Type warningType;
    const Icon icon;
};
} // namespace Beam::Diagnostic
