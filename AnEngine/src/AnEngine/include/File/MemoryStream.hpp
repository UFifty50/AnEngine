#ifndef MEMORYSTREAM_HPP
#define MEMORYSTREAM_HPP

#include <filesystem>

#include "Core/Core.hpp"
#include "File/StreamReader.hpp"
#include "File/StreamWriter.hpp"


namespace AnEngine {
    class MemoryStreamWriter final : public StreamWriter {
    public:
        MemoryStreamWriter(Buffer& buff,
                           const size_t writePos) : buffer(buff), writePos(writePos) {}

        MemoryStreamWriter() : buffer() {}

        MemoryStreamWriter(const MemoryStreamWriter&) = delete;
        ~MemoryStreamWriter() override = default;


        bool writeData(const char* data, size_t size) override;
        void flush() override { AE_CORE_ASSERT(false, "Cannot flush a memory buffer.") }
        void close() override { AE_CORE_ASSERT(false, "Cannot close a memory buffer.") }

        void seekPosition(const size_t position) override { writePos = position; }

        [[nodiscard]] size_t getSeekPosition() override { return writePos; }
        [[nodiscard]] bool isStreamBad() const override { return writePos >= buffer.size; }


        [[nodiscard]] const fs::path& getFilePath() const override {
            AE_CORE_ASSERT(false, "Cannot get path of memory buffer.")
            return fs::path();
        }

        [[nodiscard]] const std::string& getFileName() const override {
            AE_CORE_ASSERT(false, "Cannot get path of memory buffer.")
            return std::string();
        }

        [[nodiscard]] const std::string& getFileExtension() const override {
            AE_CORE_ASSERT(false, "Cannot get path of memory buffer.")
            return std::string();
        }

    private:
        Buffer& buffer;
        size_t writePos = 0;
    };

    class MemoryStreamReader final : public StreamReader {
    public:
        MemoryStreamReader(Buffer& buff);

        MemoryStreamReader(const MemoryStreamReader&) = delete;
        ~MemoryStreamReader() override;

        bool readData(byte* data, size_t size) override;
        void close() override { AE_CORE_ASSERT(false, "Cannot close a memory buffer."); }

        void seekPosition(const size_t position) override { readPos = position; }

        [[nodiscard]] size_t getSeekPosition() override { return readPos; }
        [[nodiscard]] bool isStreamBad() const override { return readPos >= buffer.size; }


        [[nodiscard]] const fs::path& getFilePath() const override {
            AE_CORE_ASSERT(false, "Cannot get path of memory buffer.")
            return fs::path();
        }

        [[nodiscard]] const std::string& getFileName() const override {
            AE_CORE_ASSERT(false, "Cannot get path of memory buffer.")
            return std::string();
        }

        [[nodiscard]] const std::string& getFileExtension() const override {
            AE_CORE_ASSERT(false, "Cannot get path of memory buffer.")
            return std::string();
        }

    private:
        Buffer& buffer;
        size_t readPos = 0;
    };
} // namespace AnEngine

#endif