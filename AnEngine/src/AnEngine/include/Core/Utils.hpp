#include "File/FileStream.hpp"


namespace AnEngine::Utils {
    template <typename T, Direction Dir>
    FileStream<Dir>& writeToStream(FileStream<Dir>& stream, const T& data) {
        stream << data;
        return stream;
    }

    template <Direction Dir>
    FileStream<Dir>& writeToStream(FileStream<Dir>& stream, const std::string& data) {
        return writeToStream(stream, data.c_str());
    }
};  // namespace AnEngine::Utils
