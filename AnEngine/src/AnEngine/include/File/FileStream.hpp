#ifndef FILESTREAM_HPP
#define FILESTREAM_HPP

#include <filesystem>
#include <fstream>

#include "Core/Core.hpp"
#include "File/StreamReader.hpp"
#include "File/StreamWriter.hpp"


namespace AnEngine {
class FileStreamWriter final : public StreamWriter {
public:
    FileStreamWriter(const std::string& path);
    FileStreamWriter(const fs::path& path);
    FileStreamWriter() : path("") {}

    FileStreamWriter(const FileStreamWriter&) = delete;
    ~FileStreamWriter() override { close(); }


    bool writeData(const char* data, size_t size) override;

    void open(const std::string& filePath) override { stream.open(filePath); }
    void close() override { stream.close(); }
    void flush() override { stream.flush(); }
    void seekPosition(size_t position) override;

    [[nodiscard]] size_t getSeekPosition() override { return stream.tellp(); }
    [[nodiscard]] bool isStreamBad() const override { return !stream.good(); }
    [[nodiscard]] bool isStreamOpen() const { return stream.is_open(); }

    [[nodiscard]] const fs::path& getFilePath() const override { return path; }

    [[nodiscard]] const std::string& getFileName() const override { return path.stem().string(); }

    [[nodiscard]] const std::string& getFileExtension() const override {
        return path.extension().string();
    }

private:
    fs::path path;
    std::ofstream stream;
};

class FileStreamReader final : public StreamReader {
public:
    FileStreamReader(const std::string& path);
    FileStreamReader(const fs::path& path);
    FileStreamReader(nullptr_t) : path("") {}

    FileStreamReader(const FileStreamReader&) = delete;
    ~FileStreamReader() override { close(); }

    void open(const std::string& filePath) override { stream.open(filePath); }
    void close() override { stream.close(); }
    void seekPosition(size_t position) override;
    uint32_t getSize() override;

    bool readData(char* data, uint32_t size) override;
    void readAll(std::string& str) override;

    [[nodiscard]] size_t getSeekPosition() override { return stream.tellg(); }
    [[nodiscard]] bool isStreamBad() const override { return !stream.good(); }
    [[nodiscard]] bool isStreamOpen() const { return stream.is_open(); }

    [[nodiscard]] const fs::path& getFilePath() const override { return path; }

    [[nodiscard]] const std::string& getFileName() const override { return path.stem().string(); }

    [[nodiscard]] const std::string& getFileExtension() const override {
        return path.extension().string();
    }

private:
    fs::path path;
    std::ifstream stream;
};
} // namespace AnEngine

#endif
