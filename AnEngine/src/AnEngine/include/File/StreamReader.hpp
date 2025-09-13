#ifndef STREAMREADER_HPP
#define STREAMREADER_HPP

#include <map>

#include "Core/Buffer.hpp"
#include "Core/Concepts.hpp"
#include "Core/Core.hpp"


namespace AnEngine {
class StreamReader {
public:
    virtual ~StreamReader() = default;

    virtual void open(const std::string& filePath) = 0;
    virtual void close() = 0;
    virtual void seekPosition(size_t position) = 0;
    [[nodiscard]] virtual size_t getSeekPosition() = 0;
    [[nodiscard]] virtual bool isStreamBad() const = 0;
    [[nodiscard]] virtual uint32_t getSize() = 0;

    operator bool() const { return !isStreamBad(); }

    virtual bool readData(char* data, uint32_t size) = 0;
    virtual void readAll(std::string& str) = 0;

    [[nodiscard]] virtual const fs::path& getFilePath() const = 0;
    [[nodiscard]] virtual const std::string& getFileName() const = 0;
    [[nodiscard]] virtual const std::string& getFileExtension() const = 0;

    void readBuffer(Buffer& buffer, const bool readSize = true) {
        if (readSize) {
            uint32_t size;
            readRaw(size);
            buffer.allocate(size);
        }
        readData(buffer.data, buffer.size);
    }

    void readString(std::string& string) {
        std::string::size_type size;
        readRaw(size);
        string.resize(size);
        readData(string.data(), size);
    }

    template <typename Type>
    void readRaw(Type& data) {
        state = readData(reinterpret_cast<char*>(&data), sizeof(Type)) ? GOOD : BAD;
        AE_CORE_ASSERT(state == GOOD, "Failed to read from file")
    }

    template <typename Type>
        requires std::is_same_v<Type, std::string>
        || Deserialisable<StreamReader, Type>
    void readObject(Type& object) {
        if constexpr (std::is_same_v<Type, std::string>()) {
            readString(object);
            return;
        }

        state = Type::Deserialise(this, object) ? GOOD : BAD;
        AE_CORE_ASSERT(state == GOOD, "Failed to read from file")
    }

    template <typename Key, typename Value>
    void readMap(std::map<Key, Value>& map, uint32_t size = 0) {
        if (size == 0) {
            readRaw<uint32_t>(size);

            map.clear();
            map.reserve(size);
        }

        for (uint32_t i = 0; i < size; i++) {
            Key key;
            Value value;

            if constexpr (std::is_trivial_v<Key>()) readRaw(key);
            else readObject(key);

            if constexpr (std::is_trivial_v<Value>()) readRaw(value);
            else readObject(value);

            map[key] = value;
        }
    }

    template <typename Key, typename Value>
    void readUMap(std::unordered_map<Key, Value>& map, uint32_t size = 0) {
        if (size == 0) {
            readRaw<uint32_t>(size);

            map.clear();
            map.reserve(size);
        }

        for (uint32_t i = 0; i < size; i++) {
            Key key;
            Value value;

            if constexpr (std::is_trivial_v<Key>()) readRaw(key);
            else readObject(key);

            if constexpr (std::is_trivial_v<Value>()) readRaw(value);
            else readObject(value);

            map[key] = value;
        }
    }

    template <typename Type>
    void readVector(std::vector<Type>& vector, uint32_t size = 0) {
        if (size == 0) {
            readRaw<uint32_t>(size);

            vector.clear();
            vector.reserve(size);
        }

        for (uint32_t i = 0; i < size; i++) {
            Type element;

            if constexpr (std::is_trivial_v<Type>()) readRaw(element);
            else readObject(element);

            vector.push_back(element);
        }
    }

private:
    enum StreamState : uint8_t {
        BAD  = 0,
        GOOD = 1,
    } state = BAD;
};
};


#endif // FILESTREAMREADER_HPP
