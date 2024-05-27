#pragma once

#include <filesystem>

#include "../io/format/display.hpp"
#include "../io/string/span.hpp"

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
class Error: IO::Format::Display {
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
        : type(type), icon(icon), span(span), message(message) {}

    explicit Error()
        : type(Type::ErrorTypeFileNotFound),
          icon(Icon::ErrorIconFileQuestionMark) {}

    unsigned char getNumber() const { return type; }

    std::string format() override, debug() override;

  private:
    std::string getTypeAsString() const;
    std::string getIcon() const;

    const Type type;
    const Icon icon;
    IO::String::Span span;
    const std::string message;
};
} // namespace Beam::Diagnostic
