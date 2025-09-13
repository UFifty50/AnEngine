#ifndef MEMORYSTREAM_HPP
#define MEMORYSTREAM_HPP

#include <filesystem>

#include "Core/Core.hpp"
#include "File/StreamReader.hpp"
#include "File/StreamWriter.hpp"


namespace AnEngine {
class MemoryStreamWriter final : public StreamWriter {
public:
    MemoryStreamWriter(const Ref<Buffer>& buff,
                       const size_t writePos) : buffer(buff), writePos(writePos) {}

    MemoryStreamWriter() = default;
    MemoryStreamWriter(const MemoryStreamWriter&) = delete;
    ~MemoryStreamWriter() override = default;


    bool writeData(const char* data, size_t size) override;
    void flush() override { AE_CORE_ASSERT(false, "Cannot flush a memory buffer.") }
    void close() override { AE_CORE_ASSERT(false, "Cannot close a memory buffer.") }

    void open(const std::string& filePath) override {
        AE_CORE_ASSERT(false, "Cannot open a memory buffer.")
    }

    void seekPosition(const size_t position) override { writePos = position; }

    [[nodiscard]] size_t getSeekPosition() override { return writePos; }
    [[nodiscard]] bool isStreamBad() const override { return writePos >= buffer.lock()->size; }


    [[nodiscard]] const fs::path& getFilePath() const override {
        AE_CORE_ASSERT(false, "Cannot get path of memory buffer.")
        return "";
    }

    [[nodiscard]] const std::string& getFileName() const override {
        AE_CORE_ASSERT(false, "Cannot get path of memory buffer.")
        return "";
    }

    [[nodiscard]] const std::string& getFileExtension() const override {
        AE_CORE_ASSERT(false, "Cannot get path of memory buffer.")
        return "";
    }

private:
    std::weak_ptr<Buffer> buffer;
    size_t writePos = 0;
};

class MemoryStreamReader final : public StreamReader {
public:
    MemoryStreamReader(const Ref<Buffer>& buff) : buffer(buff) {}

    MemoryStreamReader(const MemoryStreamReader&) = delete;
    ~MemoryStreamReader() override { buffer.lock()->release(); }

    void open(const std::string& filePath) override {
        AE_CORE_ASSERT(false, "Cannot open a memory buffer.")
    }

    void close() override { AE_CORE_ASSERT(false, "Cannot close a memory buffer."); }
    void seekPosition(const size_t position) override { readPos = position; }
    [[nodiscard]] uint32_t getSize() override { return buffer.lock()->size; }

    bool readData(char* data, uint32_t size) override;
    void readAll(std::string& str) override;

    [[nodiscard]] size_t getSeekPosition() override { return readPos; }
    [[nodiscard]] bool isStreamBad() const override { return readPos >= buffer.lock()->size; }


    [[nodiscard]] const fs::path& getFilePath() const override {
        AE_CORE_ASSERT(false, "Cannot get path of memory buffer.")
        return "";
    }

    [[nodiscard]] const std::string& getFileName() const override {
        AE_CORE_ASSERT(false, "Cannot get path of memory buffer.")
        return "";
    }

    [[nodiscard]] const std::string& getFileExtension() const override {
        AE_CORE_ASSERT(false, "Cannot get path of memory buffer.")
        return "";
    }

private:
    std::weak_ptr<Buffer> buffer;
    size_t readPos = 0;
};
} // namespace AnEngine

#endif
