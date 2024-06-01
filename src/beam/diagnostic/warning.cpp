#include <array>

#include "../../../include/beam/diagnostic/warning.hpp"
#include "../../../include/beam/io/file/reader.hpp"
#include "../../../include/beam/io/format/color/colorscheme.hpp"

std::string Beam::Diagnostic::Warning::format() {
    auto colorscheme = IO::Format::Color::Colorscheme(
        IO::Format::Types::Map<IO::Format::Types::String*,
                               IO::Format::Color::Color*>({
            {new IO::Format::Types::String("accent"),
             new IO::Format::Color::Color(
                 IO::Format::Color::Color::Type::ColorTypeYellow)},
            {new IO::Format::Types::String("type"),
             new IO::Format::Color::Color(
                 IO::Format::Color::Color::Type::ColorTypeMagenta)},
        }));

    auto reader =
        IO::File::ReaderBase<IO::File::Reader*>::New(
            "inaccessible by Beam::Diagnostic::Warning",
            getSpan().getPath().substr(0, getSpan().getPath().length() - 1))
            .getValue();

    auto previousLine = reader->readLine(*getSpan().getRow() - 2);
    auto line = reader->readLine(*getSpan().getRow());
    auto nextLine = reader->readLine(*getSpan().getRow() + 1);

    char emptiness =
        (previousLine.empty() << 2) | (line.empty() << 1) | (nextLine.empty());

    return colorscheme.color("#{accent}(" + getIconAsString() + "#)").format() +
           '\t' +
           colorscheme.color("#{type}(" + getTypeAsString() + "#)").format() +
           ' ' + getSpan().format() + ": " +
           colorscheme.color("#{accent}(" + getMessage() + "#)\n\n").format() +
           '\t' + std::to_string(*getSpan().getRow() - 1) + " │ " +
           (emptiness & 0x4 ? "..." : previousLine) + "\n" +
           colorscheme.color("#{accent}(- at#)\t").format() +
           std::to_string(*getSpan().getRow()) + " │ " +
           (emptiness & 0x2
                ? "[Failed to read] --- please report an issue."
                : colorscheme.color(line, "accent", getSpan()).format()) +
           "\n\t" + std::to_string(*getSpan().getRow() + 1) + " │ " +
           (emptiness & 0x1 ? "..." : nextLine) + '\n';
}

std::string Beam::Diagnostic::Warning::debug() {
    return "Warning(type: " + getTypeAsString() + ", icon: \"" +
           getIconAsString() + "\", span: " + getSpan().debug() +
           ", message: \"" + getMessage() + "\")";
}

std::string Beam::Diagnostic::Warning::getTypeAsString() const {
    return std::array<std::string, Type::WarningTypeCount + 1>(
               {"UnknownType", "UndeclaredName"})
        .at((unsigned char) warningType);
}

std::string Beam::Diagnostic::Warning::getIconAsString() const {
    return std::array<std::string, Icon::WarningIconCount + 1>({"", ""})
        .at((unsigned char) icon);
}
