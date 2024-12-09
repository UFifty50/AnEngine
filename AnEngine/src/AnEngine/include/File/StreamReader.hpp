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

        virtual void close() = 0;
        virtual void seekPosition(size_t position) = 0;
        [[nodiscard]] virtual size_t getSeekPosition() = 0;
        [[nodiscard]] virtual bool isStreamBad() const = 0;

        operator bool() const { return !isStreamBad(); }

        virtual bool readData(byte* data, size_t size) = 0;

        [[nodiscard]] virtual const fs::path& getFilePath() const = 0;
        [[nodiscard]] virtual const std::string& getFileName() const = 0;
        [[nodiscard]] virtual const std::string& getFileExtension() const = 0;

        void readBuffer(Buffer& buffer, bool readSize = true);
        void readString(std::string& string);

        template <typename T, typename Type = std::remove_cvref_t<T>>
        void readRaw(Type& data) {
            const bool succeeded =
                readData(reinterpret_cast<char*>(&data), sizeof(Type));
            AE_CORE_ASSERT(succeeded, "Failed to read from file");
        }

        template <typename T, typename Type = std::remove_cvref_t<T>>
            requires std::is_same_v<Type, std::string>
                     || Deserialisable<StreamReader, Type>
        void readObject(Type& object) {
            if constexpr (std::is_same_v<Type, std::string>()) {
                readString(object);
                return;
            }

            const bool succeeded = Type::Deserialise(this, object);
            AE_CORE_ASSERT(succeeded, "Failed to read from file");
        }

        template <typename Key, typename Value>
        void readMap(std::map<Key, Value>& map, const uint32_t size = 0) {
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
        void readMap(std::unordered_map<Key, Value>& map, const uint32_t size = 0) {
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

        template <typename T, typename Type = std::remove_cvref_t<T>>
        void readVector(std::vector<Type>& vector, const uint32_t size = 0) {
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
    };
};


#endif // FILESTREAMREADER_HPP