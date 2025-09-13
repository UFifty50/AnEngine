#include "aepch.hpp"

#include "File/MemoryStream.hpp"


namespace AnEngine {
// ==================== MemoryStreamReader ====================
bool MemoryStreamReader::readData(char* data, const uint32_t size) {
    const auto buf = buffer.lock();
    if (buf->size < size) buf->resize(size);

    data = buf->readBytes(size, 0);

    return true;
}

void MemoryStreamReader::readAll(std::string& str) {
    const auto buf = buffer.lock();
    str.assign(buf->data, buf->size);
}


// ==================== MemoryStreamWriter ====================
bool MemoryStreamWriter::writeData(const char* data, const size_t size) {
    const auto buf = buffer.lock();
    if (writePos + size > buf->size) {
        AE_CORE_ERROR("Buffer overflow."); // TODO: maybe increase buffer size?
        return false;
    }
    memcpy(buf->data + writePos, data, size);
    writePos += size;
    return true;
}
}; // namespace AnEngine
