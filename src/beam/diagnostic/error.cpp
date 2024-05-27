#include <array>
#include <iostream>

#include "../../../include/beam/diagnostic/error.hpp"
#include "../../../include/beam/io/file/reader.hpp"
#include "../../../include/beam/io/format/color/colorscheme.hpp"

std::string Beam::Diagnostic::Error::format() {
    auto colorscheme = IO::Format::Color::Colorscheme(
        IO::Format::Types::Map<IO::Format::Types::String*,
                               IO::Format::Color::Color*>(
            {{new IO::Format::Types::String("head"),
              new IO::Format::Color::Color(
                  IO::Format::Color::Color::Type::ColorTypeBlue)},
             {new IO::Format::Types::String("type"),
              new IO::Format::Color::Color(
                  IO::Format::Color::Color::Type::ColorTypeYellow)},
             {new IO::Format::Types::String("message"),
              new IO::Format::Color::Color(
                  IO::Format::Color::Color::Type::ColorTypeRed)}}));

    auto row = message.find("EndOfFile.") != std::string::npos
                   ? *span.getRow() - 1
                   : *span.getRow();

    auto reader = IO::File::ReaderBase<IO::File::Reader*>::New(
                      "inaccessible by Beam::Diagnostic::Error",
                      span.getPath().substr(0, span.getPath().length() - 1))
                      .getValue();

    auto previousLine = reader->readLine(row - 2);
    auto line = reader->readLine(row);
    auto nextLine = reader->readLine(row + 1);

    char emptiness =
        (previousLine.empty() << 2) | (line.empty() << 1) | (nextLine.empty());

    *span.getColumn() -= (std::to_string(row) + " | ").length();

    if (row != *span.getRow()) {
        *span.getRow() = row;
        *span.getColumn() = line.length() + 1;
        line.append("; // <- add ';'");
        *span.getLength() = 1;
    }

    *span.getColumn() += (std::to_string(row) + " | ").length();

    return colorscheme.color("#{head}(" + getIcon() + "#)").format() + '\t' +
           colorscheme.color("#{type}(" + getTypeAsString() + "#)").format() +
           ' ' + span.format() + ": " +
           colorscheme.color("#{message}(" + message + "#)\n\n").format() +
           '\t' + std::to_string(row - 1) + " | " +
           (emptiness & 0x4 ? "..." : previousLine) + "\n" +
           colorscheme.color("#{head}(- at#)\t").format() +
           std::to_string(row) + " | " +
           (emptiness & 0x2
                ? "[Failed to read] --- please report an issue."
                : colorscheme.color(line, "message", span).format()) +
           "\n\t" + std::to_string(row + 1) + " | " +
           (emptiness & 0x1 ? "..." : nextLine) + '\n';
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
