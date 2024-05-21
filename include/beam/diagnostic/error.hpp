#pragma once

#include "../io/display.hpp"
#include "../text/span.hpp"

namespace Beam::Diagnostic {
class Error: IO::Display {
  public:
    enum Type : unsigned char {
        ErrorTypeFileNotFound,
        ErrorTypeFileNotRegular,
        ErrorTypeFileNotReadable,
        ErrorTypeInvalidSyntax,
        ErrorTypeCount
    };

    enum Icon : unsigned char {
        ErrorIconFileQuestionMark,
        ErrorIconFileCross,
        ErrorIconFileLock,
        ErrorIconProgramCross,
        ErrorIconCount
    };

    Error(const Type& type, const Icon& icon, const Text::Span& span,
          const std::string& message)
        : type(type), icon(icon), span(span), message(message) {}

    explicit Error()
        : type(Type::ErrorTypeFileNotFound),
          icon(Icon::ErrorIconFileQuestionMark) {}

    std::string debug() override;

    unsigned char getNumber() const { return type; }

  private:
    std::string getTypeAsString() const;
    std::string getIcon() const;

    const Type type;
    const Icon icon;
    Text::Span span;
    const std::string message;
};
} // namespace Beam::Diagnostic
