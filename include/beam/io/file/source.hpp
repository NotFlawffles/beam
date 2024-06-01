#pragma once

#include <filesystem>

#include "reader.hpp"

namespace Beam::IO::File {
class Source {
  public:
    Source(const char* executableName, const std::filesystem::path& path)
        : path(path), reader(IO::File::ReaderBase<IO::File::Reader*>::New(
                          executableName, path)) {}

    explicit Source() {}

    std::filesystem::path getPath() const { return path; }

    Diagnostic::DiResult<IO::File::Reader*, Diagnostic::Diagnostic*>
    getReader() const {
        return reader;
    }

  private:
    const std::filesystem::path path;
    Diagnostic::DiResult<IO::File::Reader*, Diagnostic::Diagnostic*> reader;
};
} // namespace Beam::IO::File
