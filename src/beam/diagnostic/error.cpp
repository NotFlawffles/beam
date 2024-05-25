#include <array>
#include <iostream>

#include "../../../include/beam/diagnostic/error.hpp"
#include "../../../include/beam/io/format/colors.hpp"

std::string Beam::Diagnostic::Error::format() {
    auto colorscheme = IO::Format::Colors::Colorscheme(
        IO::Format::Types::Map<IO::Format::Types::String*,
                               IO::Format::Colors::Color*>(
            {{new IO::Format::Types::String("icon"),
              new IO::Format::Colors::Color(
                  IO::Format::Colors::Color::Type::ColorTypeBlue)},
             {new IO::Format::Types::String("type"),
              new IO::Format::Colors::Color(
                  IO::Format::Colors::Color::Type::ColorTypeYellow)},
             {new IO::Format::Types::String("message"),
              new IO::Format::Colors::Color(
                  IO::Format::Colors::Color::Type::ColorTypeRed)}}));

    return colorscheme.color("#{icon}(" + getIcon() + "#)").format() + '\t' +
           colorscheme.color("#{type}(" + getTypeAsString() + "#)").format() +
           ' ' + span.format() + ": " +
           colorscheme.color("#{message}(" + message + "#)").format();
}

std::string Beam::Diagnostic::Error::debug() {
    return "Error(type: " + getTypeAsString() + ", icon: \"" + getIcon() +
           "\", span: " + span.debug() + ", message: \"" + message + "\")";
}

std::string Beam::Diagnostic::Error::getTypeAsString() const {
    return std::array<std::string, Type::ErrorTypeCount + 1> {
        "FileNotFound",  "FileNotRegular",  "FileNotReadable",
        "InvalidSyntax", "UnexpectedToken", "ColorNotFound"}
        .at(type);
}

std::string Beam::Diagnostic::Error::getIcon() const {
    return std::array<std::string, Icon::ErrorIconCount + 1> {
        "", "󰮘", "󰈡", "󰺬", "", "󰹊"}
        .at(icon);
}
