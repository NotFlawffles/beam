#include <array>
#include <iostream>

#include "../../../include/beam/diagnostic/error.hpp"
#include "../../../include/beam/io/file/reader.hpp"
#include "../../../include/beam/io/format/color/colorscheme.hpp"

std::string Beam::Diagnostic::Error::format() {
    auto colorscheme = IO::Format::Color::Colorscheme(
        IO::Format::Types::Map<IO::Format::Types::String*,
                               IO::Format::Color::Color*>({
            {new IO::Format::Types::String("accent"),
             new IO::Format::Color::Color(
                 IO::Format::Color::Color::Type::ColorTypeRed)},
            {new IO::Format::Types::String("type"),
             new IO::Format::Color::Color(
                 IO::Format::Color::Color::Type::ColorTypeMagenta)},
        }));

    auto row = getMessage().find("EndOfFile.") != std::string::npos
                   ? *getSpan().getRow() - 1
                   : *getSpan().getRow();

    if (showSource) {
        auto reader =
            IO::File::ReaderBase<IO::File::Reader*>::New(
                "inaccessible by Beam::Diagnostic::Error",
                getSpan().getPath().substr(0, getSpan().getPath().length() - 1))
                .getValue();

        auto previousLine = reader->readLine(row - 1);
        auto line = reader->readLine(row);
        auto nextLine = reader->readLine(row + 1);

        char emptiness = (previousLine.empty() << 2) | (line.empty() << 1) |
                         (nextLine.empty());

        if (row != *getSpan().getRow()) {
            *getSpan().getRow() = row;
            *getSpan().getColumn() = line.length() + 1;
            line.append("; // <- add ';'");
            *getSpan().getLength() = 1;
        }

        return colorscheme.color("#{accent}(" + getIconAsString() + "#)")
                   .format() +
               '\t' +
               colorscheme.color("#{type}(" + getTypeAsString() + "#)")
                   .format() +
               ' ' + getSpan().format() + ": " +
               colorscheme.color("#{accent}(" + getMessage() + "#)\n\n")
                   .format() +
               '\t' + std::to_string(row - 1) + " │ " +
               (emptiness & 0x4 ? "..." : previousLine) + "\n" +
               colorscheme.color("#{accent}(- at#)\t").format() +
               std::to_string(row) + " │ " +
               (emptiness & 0x2
                    ? "[Failed to read] --- please report an issue."
                    : colorscheme.color(line, "accent", getSpan()).format()) +
               "\n\t" + std::to_string(row + 1) + " │ " +
               (emptiness & 0x1 ? "..." : nextLine) + '\n';
    }

    return colorscheme.color("#{accent}(" + getIconAsString() + "#)").format() +
           '\t' +
           colorscheme.color("#{type}(" + getTypeAsString() + "#)").format() +
           ' ' + getSpan().format() + ": " +
           colorscheme.color("#{accent}(" + getMessage() + "#)\n").format();
}

std::string Beam::Diagnostic::Error::debug() {
    return "Error(type: " + getTypeAsString() + ", icon: \"" +
           getIconAsString() + "\", span: " + getSpan().debug() +
           ", message: \"" + getMessage() + "\")";
}

std::string Beam::Diagnostic::Error::getTypeAsString() const {
    return std::array<std::string, Type::ErrorTypeCount + 1> {
        "FileNotFound",    "FileNotRegular", "FileNotReadable", "InvalidSyntax",
        "UnexpectedToken", "ColorNotFound",  "MismatchedTypes", "Redefinition"}
        .at(errorType);
}

std::string Beam::Diagnostic::Error::getIconAsString() const {
    return std::array<std::string, Icon::ErrorIconCount + 1> {
        "", "󰮘", "󰈡", "󰺬", "", "󰹊", "󱄑"}
        .at(icon);
}
