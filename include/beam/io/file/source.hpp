#pragma once

#include <filesystem>

#include "reader.hpp"

namespace Beam::IO::File {
class Source {
  public:
    Source(const char* executableName, const std::filesystem::path& path)
        : path(path), reader(IO::File::ReaderBase<IO::File::Reader>::New(
                          executableName, path)) {}

    std::filesystem::path getPath() const { return path; }

    Diagnostic::Result<IO::File::Reader> getReader() const { return reader; }

  private:
    const std::filesystem::path path;
    Diagnostic::Result<IO::File::Reader> reader;
};
} // namespace Beam::IO::File
