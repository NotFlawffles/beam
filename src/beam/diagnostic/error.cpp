#include <array>
#include <iostream>

#include "../../../include/beam/diagnostic/error.hpp"
#include "../../../include/beam/io/file/reader.hpp"
#include "../../../include/beam/io/format/colors.hpp"

std::string Beam::Diagnostic::Error::format() {
    auto colorscheme = IO::Format::Colors::Colorscheme(
        IO::Format::Types::Map<IO::Format::Types::String*,
                               IO::Format::Colors::Color*>(
            {{new IO::Format::Types::String("head"),
              new IO::Format::Colors::Color(
                  IO::Format::Colors::Color::Type::ColorTypeBlue)},
             {new IO::Format::Types::String("type"),
              new IO::Format::Colors::Color(
                  IO::Format::Colors::Color::Type::ColorTypeYellow)},
             {new IO::Format::Types::String("message"),
              new IO::Format::Colors::Color(
                  IO::Format::Colors::Color::Type::ColorTypeRed)}}));

    auto row = message.find("EndOfFile.") != std::string::npos
                   ? *span.getRow() - 1
                   : *span.getRow();

    auto line = IO::File::ReaderBase<IO::File::Reader*>::New(
                    "inaccessible by Beam::Diagnostic::Error",
                    span.getPath().substr(0, span.getPath().length() - 1))
                    .getValue()
                    ->readLine(row);

    if (row != *span.getRow()) {
        *span.getRow() = row;
        *span.getColumn() = line.length() + 1;
        line.append("; // <- add ';'");
        *span.getLength() = 1;
    }

    return colorscheme.color("#{head}(" + getIcon() + "#)").format() + '\t' +
           colorscheme.color("#{type}(" + getTypeAsString() + "#)").format() +
           ' ' + span.format() + ": " +
           colorscheme.color("#{message}(" + message + "#)\n\n#{head}(- at#)\t")
               .format() +
           (line.empty() ? "inaccessible"
                         : colorscheme.color(line, "message", span).format()) +
           '\n';
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
