#ifndef STREAMWRITER_HPP
#define STREAMWRITER_HPP

#include <fstream>
#include <map>
#include <string>
#include <unordered_map>

#include "Core/Buffer.hpp"
#include "Core/Core.hpp"


namespace AnEngine {
class StreamWriter {
public:
    virtual ~StreamWriter() = default;

    virtual void open(const std::string& filePath) = 0;
    virtual void close() = 0;
    virtual void flush() = 0;
    virtual void seekPosition(size_t position) = 0;
    [[nodiscard]] virtual size_t getSeekPosition() = 0;
    [[nodiscard]] virtual bool isStreamBad() const = 0;

    operator bool() const { return !isStreamBad(); }

    virtual void writeBuffer(const Buffer& buffer, bool writeSize = true) {
        writeRaw(buffer.size);
        writeData(buffer.data, buffer.size);
    }

    virtual void writeString(const std::string& string) {
        writeRaw(static_cast<uint32_t>(string.size()));
        writeData(string.data(), string.size());
    }

    virtual bool writeData(const char* data, size_t size) = 0;

    [[nodiscard]] virtual const fs::path& getFilePath() const = 0;
    [[nodiscard]] virtual const std::string& getFileName() const = 0;
    [[nodiscard]] virtual const std::string& getFileExtension() const = 0;

    template <typename Type>
    void writeRaw(const Type& data) {
        state = writeData(reinterpret_cast<const char*>(&data), sizeof(Type)) ? GOOD : BAD;
        AE_CORE_ASSERT(state == GOOD, "Failed to write to file");
    }

    template <typename Type>
        requires std::is_same_v<Type, std::string>
        || Serialisable<StreamWriter, Type>
    void writeObject(const Type& object) {
        if constexpr (std::is_same_v<Type, std::string>()) {
            writeString(object);
            return;
        }

        state = Type::Serialise(this, object) ? GOOD : BAD;
        AE_CORE_ASSERT(state == GOOD, "Failed to write to file");
    }

    template <typename Key, typename Value>
    void writeMap(const std::map<Key, Value>& map, const bool writeSize = true) {
        if (writeSize) { writeRaw(static_cast<uint32_t>(map.size())); }

        for (const auto& [key, value] : map) {
            if constexpr (std::is_trivial_v<Key>()) writeRaw(key);
            else writeObject(key);

            if constexpr (std::is_trivial_v<Value>()) writeRaw(value);
            else writeObject(value);
        }
    }

    template <typename Key, typename Value>
    void writeUMap(const std::unordered_map<Key, Value>& map, const bool writeSize = true) {
        if (writeSize) { writeRaw(static_cast<uint32_t>(map.size())); }

        for (const auto& [key, value] : map) {
            if constexpr (std::is_trivial_v<Key>()) writeRaw(key);
            else writeObject(key);

            if constexpr (std::is_trivial_v<Value>()) writeRaw(value);
            else writeObject(value);
        }
    }

    template <typename Type>
    void writeVector(const std::vector<Type>& vector, const bool writeSize = true) {
        if (writeSize) { writeRaw(static_cast<uint32_t>(vector.size())); }

        for (const Type& element : vector) {
            if constexpr (std::is_trivial_v<Type>()) writeRaw(element);
            else writeObject(element);
        }
    }

private:
    enum StreamState : uint8_t {
        BAD  = 0,
        GOOD = 1,
    } state = BAD;
};
} // namespace AnEngine

#endif
