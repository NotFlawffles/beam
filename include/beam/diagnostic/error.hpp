#pragma once

#include <filesystem>

#include "../io/format/display.hpp"
#include "../io/string/span.hpp"
#include "diagnostic.hpp"

#define DefineAndReturnIfError(name, value)                                    \
    auto name = value;                                                         \
    if (name.isFailure())                                                      \
    return name.getError()

#define DefineAndReturnVecIfError(name, errorType, value)                      \
    auto name = value;                                                         \
    if (name.isFailure())                                                      \
    return new Vec(errorType, {name.getError()})

#define DefineAndReturnVecIfErrorAndSkip(name, errorType, value)               \
    auto name = value;                                                         \
    skip();                                                                    \
    if (name.isFailure())                                                      \
    return new Vec(errorType, {name.getError()})

namespace Beam::Diagnostic {
class Error: public Diagnostic {
  public:
    enum Type : unsigned char {
        ErrorTypeFileNotFound,
        ErrorTypeFileNotRegular,
        ErrorTypeFileNotReadable,
        ErrorTypeInvalidSyntax,
        ErrorTypeUnexpectedToken,
        ErrorTypeColorNotFound,
        ErrorTypeCount
    };

    enum Icon : unsigned char {
        ErrorIconFileQuestionMark,
        ErrorIconFileCross,
        ErrorIconFileLock,
        ErrorIconProgramCross,
        ErrorIconCurlyBracesCross,
        ErrorIconColorLine,
        ErrorIconCount
    };

    Error(const Type& type, const Icon& icon, const IO::String::Span& span,
          const std::string& message)
        : Diagnostic(Diagnostic::Type::Error, span, message), errorType(type),
          icon(icon) {}

    explicit Error()
        : Diagnostic(Diagnostic::Type::Error),
          errorType(Type::ErrorTypeFileNotFound),
          icon(Icon::ErrorIconFileQuestionMark) {}

    unsigned char getNumber() const { return errorType; }

    std::string getTypeAsString() const, getIconAsString() const,
        format() override, debug() override;

  private:
    const Type errorType;
    const Icon icon;
};
} // namespace Beam::Diagnostic
