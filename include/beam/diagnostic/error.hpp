#pragma once

#include "../io/format/display/debugger.hpp"
#include "../io/string/span.hpp"

namespace Beam::Diagnostic {
class Error: IO::Format::Display::Debugger {
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

    Error(const Type& type, const Icon& icon, const IO::String::Span& span,
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
    IO::String::Span span;
    const std::string message;
};
} // namespace Beam::Diagnostic
