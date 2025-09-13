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

bool FileStreamReader::readData(char* data, const uint32_t size) {
    stream.read(data, size);
    return !stream.fail();
}

void FileStreamReader::readAll(std::string& str) {
    str.assign(std::istreambuf_iterator(stream), std::istreambuf_iterator<char>());
}

void FileStreamReader::seekPosition(const size_t position) {
    stream.seekg(static_cast<std::streamoff>(position));
}

uint32_t FileStreamReader::getSize() {
    stream.seekg(0, std::ios::end);
    const uint32_t size = stream.tellg();
    stream.seekg(0, std::ios::beg);

    return size;
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

void FileStreamWriter::seekPosition(const size_t position) {
    stream.seekp(static_cast<std::streamoff>(position));
}
} // namespace AnEngine
