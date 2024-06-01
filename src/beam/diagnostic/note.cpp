#include "../../../include/beam/diagnostic/note.hpp"
#include "../../../include/beam/io/file/reader.hpp"
#include "../../../include/beam/io/format/color/colorscheme.hpp"

std::string Beam::Diagnostic::Note::format() {
    return IO::Format::Color::Colorscheme(
               IO::Format::Types::Map<IO::Format::Types::String*,
                                      IO::Format::Color::Color*>(
                   {{new IO::Format::Types::String("asterisk"),
                     new IO::Format::Color::Color(
                         IO::Format::Color::Color::Type::ColorTypeBlue)},
                    {new IO::Format::Types::String("note"),
                     new IO::Format::Color::Color(
                         IO::Format::Color::Color::Type::ColorTypeCyan)}}))
               .color("#{asterisk}(*#) #{note}(" + getMessage() + '\n' +
                      (showLine
                           ? "- at\t" + std::to_string(*getSpan().getRow()) +
                                 " │ " +
                                 IO::File::ReaderBase<IO::File::Reader*>::New(
                                     "inaccessible by Beam::Diagnostic::Note",
                                     getSpan().getPath().substr(
                                         0, getSpan().getPath().length() - 1))
                                     .getValue()
                                     ->readLine(*getSpan().getRow())
                           : "") +
                      "#)")
               .format() +
           '\n';
}

std::string Beam::Diagnostic::Note::debug() { return "Note(...)"; }
