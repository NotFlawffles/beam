#pragma once

#include <filesystem>

#include "../io/reader.hpp"

namespace Beam::Text {
class Source {
  public:
    Source(const char* executableName, const std::filesystem::path& path)
        : path(path),
          reader(IO::ReaderBase<IO::Reader>::New(executableName, path)) {}

    std::filesystem::path getPath() const { return path; }

    Diagnostic::Result<IO::Reader> getReader() const { return reader; }

  private:
    const std::filesystem::path path;
    Diagnostic::Result<IO::Reader> reader;
};
} // namespace Beam::Text
