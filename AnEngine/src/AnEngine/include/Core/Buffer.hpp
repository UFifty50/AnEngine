#ifndef BUFFER_HPP
#define BUFFER_HPP
#include "Core.hpp"

namespace AnEngine {
struct Buffer {
    uint32_t size;
    char* data;

    Buffer() : size(0), data(nullptr) {}
    Buffer(char* data, const size_t size) : size(size), data(data) {}
    ~Buffer() { release(); }

    void allocate(const size_t newSize) {
        delete[] data;
        data = nullptr;

        if (newSize == 0) return;

        data = new char[newSize];
        size = newSize;
    }

    void resize(const uint32_t newSize) {
        if (size == newSize) return;
        if (newSize == 0) {
            release();
            return;
        }

        auto newData = new char[newSize];
        if (data != nullptr) {
            memcpy(newData, data, std::min(size, newSize));
            delete[] data;
        }

        data = newData;
        size = newSize;
    }

    void release() {
        delete[] data;
        data = nullptr;
        size = 0;
    }

    template <typename T, typename Type = std::remove_cvref_t<T>>
    Type& read(const uint32_t offset = 0) {
        AE_CORE_ASSERT(data && size > 0, "Buffer is empty")
        AE_CORE_ASSERT(offset + sizeof(Type) <= size, "Buffer overflow")

        Type* result = reinterpret_cast<Type*>(data + offset);
        return *result;
    }

    template <typename T, typename Type = std::remove_cvref_t<T>>
    const Type& read(const uint32_t offset = 0) const {
        AE_CORE_ASSERT(data && size > 0, "Buffer is empty")
        AE_CORE_ASSERT(offset + sizeof(Type) <= size, "Buffer overflow")

        const Type* result = reinterpret_cast<const Type*>(data + offset);
        return *result;
    }

    [[nodiscard]] char* readBytes(const uint32_t readSize, const uint32_t offset) const {
        AE_CORE_ASSERT(data && size > 0, "Buffer is empty")
        AE_CORE_ASSERT(offset + readSize <= size, "Buffer overflow")

        const auto buffer = new char[size];
        memcpy(data + offset, data, size);
        return buffer;
    }

    char& operator[](const uint32_t index) const {
        AE_CORE_ASSERT(data && size > 0, "Buffer is empty")
        AE_CORE_ASSERT(index < size, "Buffer overflow")

        return data[index];
    }

    static Buffer Copy(const Buffer& other) {
        Buffer buffer;
        buffer.allocate(other.size);
        memcpy(buffer.data, other.data, other.size);
        return buffer;
    }

    static Buffer Copy(const void* data, const size_t size) {
        Buffer buffer;
        buffer.allocate(size);
        memcpy(buffer.data, data, size);
        return buffer;
    }
};
}
#endif
