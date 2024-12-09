#include "aepch.hpp"

#include "File/FileStream.hpp"

#include <string>

#include "Core/Core.hpp"


namespace AnEngine {
    // ==================================== FileStreamReader ====================================
    FileStreamReader::FileStreamReader(const fs::path& path)
        : path(path), stream(path, std::ios::in | std::ios::binary) {}

    FileStreamReader::FileStreamReader(const std::string& path)
        : path(path), stream(path, std::ios::in | std::ios::binary) {}

    bool FileStreamReader::readData(byte* data, const size_t size) override {
        stream.read(reinterpret_cast<char*>(data), static_cast<std::streamsize>(size));
        return !stream.fail();
    }

    void FileStreamReader::seekPosition(const size_t position) override {
        stream.seekg(static_cast<std::streamoff>(position));
    }


    // ==================================== FileStreamWriter ====================================
    FileStreamWriter::FileStreamWriter(const fs::path& path)
        : path(path), stream(path, std::ios::out | std::ios::binary) {}

    FileStreamWriter::FileStreamWriter(const std::string& path)
        : path(path), stream(path, std::ios::out | std::ios::binary) {}

    bool FileStreamWriter::writeData(const char* data, const size_t size) {
        stream.write(data, static_cast<std::streamsize>(size));
        return !stream.fail();
    }

    void FileStreamWriter::seekPosition(size_t position) {
        stream.seekp(static_cast<std::streamoff>(position));
    }
} // namespace AnEngine