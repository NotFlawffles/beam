#pragma once

#include <string>

#include "../../io/format/display/debugger.hpp"
#include "../../io/format/display/formatter.hpp"

namespace Beam::IO::String {
class Span: Format::Display::Formatter, Format::Display::Debugger {
  public:
    Span(const std::string& stream, const std::string& path,
         const unsigned long long int& index, const unsigned long long int& row,
         const unsigned long long int& column,
         const unsigned long long int& length)
        : stream(stream), path(path), index(index), row(row), column(column),
          length(length) {}

    explicit Span() {}

    const std::string getStream() const { return stream; }

    const std::string getPath() const { return path; }

    unsigned long long int* getIndex() { return &index; }

    unsigned long long int* getRow() { return &row; }

    unsigned long long int* getColumn() { return &column; }

    unsigned long long int* getLength() { return &length; }

    Span clone() { return Span(*this); }

    Span withLengthOf(const unsigned long long int& newLength) {
        length = newLength;
        return *this;
    }

    Span withColumnOf(const unsigned long long int& newColumn) {
        column = newColumn;
        return *this;
    }

    std::string format() override;
    std::string debug() override;

  private:
    const std::string stream, path;
    unsigned long long int index, row, column, length;
};
} // namespace Beam::IO::String
