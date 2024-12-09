#ifndef BUFFER_HPP
#define BUFFER_HPP
#include "Core.hpp"

namespace AnEngine {
    struct Buffer {
        void* data;
        size_t size;

        Buffer() : data(nullptr), size(0) {}
        Buffer(void* data, const size_t size) : data(data), size(size) {}
        ~Buffer() { release(); }

        void allocate(const size_t newSize) {
            delete[] static_cast<byte*>(data);
            data = nullptr;

            if (newSize == 0) return;

            data = new char[newSize];
            size = newSize;
        }

        void resize(const size_t newSize) {
            if (size == newSize) return;
            if (newSize == 0) {
                release();
                return;
            }

            void* newData = new char[newSize];
            if (data != nullptr) {
                memcpy(newData, data, std::min(size, newSize));
                delete[] static_cast<byte*>(data);
            }

            data = newData;
            size = newSize;
        }

        void release() {
            delete[] static_cast<byte*>(data);
            data = nullptr;
            size = 0;
        }

        template <typename T, typename Type = std::remove_cvref_t<T>>
        Type& read(const size_t offset = 0) {
            Type* result = reinterpret_cast<Type*>(static_cast<byte*>(data) + offset);
            return *result;
        }

        template <typename T, typename Type = std::remove_cvref_t<T>>
        const Type& read(const size_t offset = 0) const {
            const Type* result = reinterpret_cast<const Type*>(
                static_cast<byte*>(data) + offset);
            return *result;
        }

        [[nodiscard]] byte* readBytes(const size_t readSize, const size_t offset) const {
            AE_CORE_ASSERT(offset + readSize <= size, "Buffer overflow")
            const auto buffer = new byte[size];
            memcpy(static_cast<byte*>(data) + offset, data, size);
            return buffer;
        }

        byte& operator[](const size_t index) const { return static_cast<byte*>(data)[index]; }

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