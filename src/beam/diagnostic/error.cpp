#include <array>
#include <iostream>

#include "../../../include/beam/diagnostic/error.hpp"

std::string Beam::Diagnostic::Error::format() {
    return getIcon() + '\t' + getTypeAsString() + ' ' + span.format() + ": " +
           message;
}

std::string Beam::Diagnostic::Error::debug() {
    return "Error(type: " + getTypeAsString() + ", icon: \"" + getIcon() +
           "\", span: " + span.debug() + ", message: \"" + message + "\")";
}

std::string Beam::Diagnostic::Error::getTypeAsString() const {
    return std::array<std::string, Type::ErrorTypeCount + 1> {
        "FileNotFound", "FileNotRegular", "FileNotReadable", "InvalidSyntax"}
        .at(type);
}

std::string Beam::Diagnostic::Error::getIcon() const {
    return std::array<std::string, Icon::ErrorIconCount + 1> {"", "󰮘",
                                                              "󰈡", "󰺬"}
        .at(icon);
}
