#pragma once

#include <filesystem>
#include <fstream>

#include "../../diagnostic/error.hpp"
#include "../../diagnostic/result.hpp"
#include "../../io/format/display.hpp"

namespace Beam::IO::File {
class Reader: Format::Display {
  public:
    Reader(const std::filesystem::path& path, const std::string& content)
        : path(path), content(content) {}

    explicit Reader() {}

    std::string readAll() const, readLine(const unsigned long long int& index);

    std::string format() override, debug() override;

  private:
    const std::string path;
    const std::string content;
};

template<typename Derived> class ReaderBase: Format::Display {
  public:
    static Diagnostic::Result<Derived> New(const char* executableName,
                                           const std::filesystem::path& path) {

        auto fileNameStart = std::string(executableName).length() + 1;
        const auto pathAsString = std::string(path);

        if (!std::filesystem::exists(path)) {
            return new Diagnostic::Error(
                Diagnostic::Error::Type::ErrorTypeFileNotFound,
                Diagnostic::Error::Icon::ErrorIconFileQuestionMark,
                IO::String::Span("command line input", "", fileNameStart, 1,
                                 fileNameStart + 1, pathAsString.length()),
                "could not find file `" + pathAsString + '`');
        } else if (!std::filesystem::is_regular_file(path)) {
            return new Diagnostic::Error(
                Diagnostic::Error::Type::ErrorTypeFileNotRegular,
                Diagnostic::Error::Icon::ErrorIconFileCross,
                IO::String::Span("command line input", "", fileNameStart, 1,
                                 fileNameStart + 1, pathAsString.length()),
                "specified file is not a regular `" + pathAsString + '`');
        } else if ((std::filesystem::status(path).permissions() &
                    std::filesystem::perms::others_read) ==
                   std::filesystem::perms::none) {
            return new Diagnostic::Error(
                Diagnostic::Error::Type::ErrorTypeFileNotReadable,
                Diagnostic::Error::Icon::ErrorIconFileLock,
                IO::String::Span("command line input", "", fileNameStart, 1,
                                 fileNameStart + 1, pathAsString.length()),
                "could not read file `" + pathAsString + '`');
        }

        std::ifstream file(path);
        std::stringstream stream;
        stream << file.rdbuf();
        return new Reader(path, stream.str());
    }
};
} // namespace Beam::IO::File
