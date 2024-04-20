#ifndef UUID_HPP
#define UUID_HPP

#include <cstddef>
#include <cstdint>
#include <sstream>
#include <string>

#include "Core/Random.hpp"
#include "Core/Utils/Utils.hpp"


namespace AnEngine {
    struct UUID {
        uint32_t id[2];

        static UUID fromStr(const std::string& str) {
            uint32_t id[2];

            id[0] = std::stoul(str.substr(0, 8), nullptr, 16);
            id[1] = std::stoul(str.substr(9, 8), nullptr, 16);

            return UUID(id[0], id[1]);
        }

        UUID() {
            id[0] = AnEngine::Random::getInt();
            id[1] = AnEngine::Random::getInt();
        }

        UUID(const UUID& other) = default;

        UUID(uint32_t id0, uint32_t id1) {
            id[0] = id0;
            id[1] = id1;
        }

        UUID(nullptr_t) {
            id[0] = 0;
            id[1] = 0;
        }

        bool isNull() const { return id[0] == 0 || id[1] == 0; }

        operator std::string() const {
            std::string hexStr[2];
            std::stringstream sslhs;
            std::stringstream ssrhs;

            for (int i = 0; i < 2; ++i) {
                std::stringstream ss;
                ss << std::hex << id[i];
                hexStr[i] = ss.str();
                hexStr[i].insert(0, 8 - hexStr[i].length(), '0');
            }

            return hexStr[0] + "-" + hexStr[1];
        }

        bool operator==(const UUID& other) const {
            return id[0] == other.id[0] && id[1] == other.id[1];
        }
    };
}  // namespace AnEngine

struct UUIDHasher {
    size_t operator()(const AnEngine::UUID& uuid) const {
        size_t hash1 = std::hash<uint32_t>{}(uuid.id[0]);
        size_t hash2 = std::hash<uint32_t>{}(uuid.id[1]);
        return AnEngine::hash_combine(hash1, hash2);
    }
};

#endif
