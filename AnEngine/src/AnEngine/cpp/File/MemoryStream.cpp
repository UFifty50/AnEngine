#include "aepch.hpp"

#include "File/MemoryStream.hpp"


namespace AnEngine {
    // ==================== MemoryStreamReader ====================
    MemoryStreamReader::MemoryStreamReader(Buffer& buff) : buffer(buff) {}
    MemoryStreamReader::~MemoryStreamReader() { buffer.release(); }

    bool MemoryStreamReader::readData(byte* data, const size_t size) {
        if (buffer.size < size) buffer.resize(size);

        data = buffer.readBytes(size, 0);

        return true;
    }


    // ==================== MemoryStreamWriter ====================
    bool MemoryStreamWriter::writeData(const char* data, const size_t size) {
        if (writePos + size > buffer.size) {
            AE_CORE_ERROR("Buffer overflow."); // TODO: maybe increase buffer size?
            return false;
        }
        memcpy(buffer.data + writePos, data, size);
        writePos += size;
        return true;
    }
}; // namespace AnEngine