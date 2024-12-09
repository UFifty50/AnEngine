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

        virtual void close() = 0;
        virtual void flush() = 0;
        virtual void seekPosition(size_t position) = 0;
        [[nodiscard]] virtual size_t getSeekPosition() = 0;
        [[nodiscard]] virtual bool isStreamBad() const = 0;

        operator bool() const { return !isStreamBad(); }

        virtual bool writeData(const char* data, size_t size) = 0;

        [[nodiscard]] virtual const fs::path& getFilePath() const = 0;
        [[nodiscard]] virtual const std::string& getFileName() const = 0;
        [[nodiscard]] virtual const std::string& getFileExtension() const = 0;


        void writeBuffer(const Buffer& buffer, bool writeSize = true);
        void writeString(const std::string& string);

        template <typename T, typename Type = std::remove_cvref_t<T>>
        void writeRaw(const Type& data) {
            const bool succeeded =
                writeData(reinterpret_cast<const char*>(&data), sizeof(Type));
            AE_CORE_ASSERT(succeeded, "Failed to write to file");
        }

        template <typename T, typename Type = std::remove_cvref_t<T>>
            requires std::is_same_v<Type, std::string>
                     || Serialisable<StreamWriter, Type>
        void writeObject(const Type& object) {
            if constexpr (std::is_same_v<Type, std::string>()) {
                writeString(object);
                return;
            }

            const bool succeeded = Type::Serialise(this, object);
            AE_CORE_ASSERT(succeeded, "Failed to write to file");
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
        void writeMap(const std::unordered_map<Key, Value>& map, const bool writeSize = true) {
            if (writeSize) { writeRaw(static_cast<uint32_t>(map.size())); }

            for (const auto& [key, value] : map) {
                if constexpr (std::is_trivial_v<Key>()) writeRaw(key);
                else writeObject(key);

                if constexpr (std::is_trivial_v<Value>()) writeRaw(value);
                else writeObject(value);
            }
        }

        template <typename T, typename Type = std::remove_cvref_t<T>>
        void writeVector(const std::vector<Type>& vector, const bool writeSize = true) {
            if (writeSize) { writeRaw(static_cast<uint32_t>(vector.size())); }

            for (const Type& element : vector) {
                if constexpr (std::is_trivial_v<Type>()) writeRaw(element);
                else writeObject(element);
            }
        }
    };
} // namespace AnEngine

#endif