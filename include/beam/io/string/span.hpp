#pragma once

#include <string>

#include "../../io/format/display/debugger.hpp"

namespace Beam::IO::String {
class Span: Format::Display::Debugger {
  public:
    Span(const std::string& stream, const unsigned long long int& index,
         const unsigned long long int& row,
         const unsigned long long int& column,
         const unsigned long long int& length)
        : stream(stream), index(index), row(row), column(column),
          length(length) {}

    explicit Span() {}

    const std::string getStream() const { return stream; }

    unsigned long long int* getIndex() { return &index; }

    unsigned long long int* getRow() { return &row; }

    unsigned long long int* getColumn() { return &column; }

    unsigned long long int* getLength() { return &length; }

    Span clone() { return Span(*this); }

    Span withLengthOf(const unsigned long long int& newLength) {
        length = newLength;
        return *this;
    }

    std::string debug() override;

  private:
    const std::string stream;
    unsigned long long int index, row, column, length;
};
} // namespace Beam::IO::String
