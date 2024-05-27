#include "../../../../include/beam/io/format/colors.hpp"

Beam::Diagnostic::DiResult<Beam::IO::Format::Types::String*,
                           Beam::Diagnostic::Error*>
Beam::IO::Format::Colors::Colorscheme::color(const std::string& target) const {
    auto span = String::Span("color parser", "", 0, 1, 1, target.length());

    auto current = target[*span.getIndex()];
    auto result = new Types::String("");

    while (*span.getIndex() < target.length()) {
        if (current == '#') {
            if (target[*span.getIndex() + 1] == '{') {
                current = target[*span.getIndex() += 2];
                auto currentColorSequenceKey = std::string();
                auto currentColorSequenceValue = std::string();

                while (*span.getIndex() < target.length() && current != '}') {
                    currentColorSequenceKey.push_back(current);
                    current = target[*span.getIndex() += 1];
                }

                if (current != '}') {
                    return new Diagnostic::Error(
                        Diagnostic::Error::Type::ErrorTypeInvalidSyntax,
                        Diagnostic::Error::Icon::ErrorIconProgramCross,
                        span.withColumnOf(*span.getIndex() + 1),
                        "unterminated color sequence near key");
                }

                if ((current = target[*span.getIndex() += 1]) != '(') {
                    return new Diagnostic::Error(
                        Diagnostic::Error::Type::ErrorTypeUnexpectedToken,
                        Diagnostic::Error::Icon::ErrorIconProgramCross,
                        span.withColumnOf(*span.getIndex() + 1),
                        "expected '(' after color sequence start");
                }

                current = target[*span.getIndex() += 1];

                while (*span.getIndex() < target.length() && current != '#') {
                    currentColorSequenceValue.push_back(current);
                    current = target[*span.getIndex() += 1];
                }

                if (current != '#' &&
                    (current = target[*span.getIndex() += 1]) != ')') {
                    return new Diagnostic::Error(
                        Diagnostic::Error::Type::ErrorTypeInvalidSyntax,
                        Diagnostic::Error::Icon::ErrorIconProgramCross,
                        span.withColumnOf(*span.getIndex() + 1),
                        "unterminated color sequence "
                        "near value");
                }

                current = target[*span.getIndex() += 1];

                if (colors.has(new Types::String(currentColorSequenceKey))) {
                    result->append(
                        "\x1b" +
                        colors.get(new Types::String(currentColorSequenceKey))
                            ->get() +
                        currentColorSequenceValue + "\x1b" +
                        Color(Color::Type::ColorTypeReset).get());
                } else {
                    return new Diagnostic::Error(
                        Diagnostic::Error::Type::ErrorTypeColorNotFound,
                        Diagnostic::Error::Icon::ErrorIconColorLine,
                        span.withColumnOf(*span.getIndex() + 1),
                        "could not find color `" + currentColorSequenceValue +
                            '`');
                }
            }
        } else {
            result->push_back(current);
        }

        current = target[*span.getIndex() += 1];
    }

    return result;
}

Beam::Diagnostic::DiResult<Beam::IO::Format::Types::String*,
                           Beam::Diagnostic::Error*>
Beam::IO::Format::Colors::Colorscheme::color(const std::string& target,
                                             const std::string& which,
                                             String::Span& span) const {
    auto copy = target;

    copy.insert(*span.getColumn() - 1, "#{" + which + "}(");
    copy.insert(*span.getColumn() - 1 + *span.getLength() +
                    ("#{" + which + "})").length(),
                "#)");

    return color(copy);
}
