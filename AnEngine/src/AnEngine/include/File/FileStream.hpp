#ifndef FILESTREAM_HPP
#define FILESTREAM_HPP

#include <fstream>
#include <string>

namespace AnEngine {
    enum class Direction { Input, Output };


    template <Direction dir>
    class FileStream;

    template <>
    class FileStream<Direction::Input> {
    public:
        virtual ~FileStream(){};

        virtual const std::string readAll() const = 0;
        virtual void close() = 0;

        virtual const std::string& getFilePath() const = 0;
        virtual const std::string& getFileName() const = 0;
        virtual const std::string& getFileExtension() const = 0;
    };

    template <>
    class FileStream<Direction::Output> {
    public:
        virtual ~FileStream(){};

        virtual void writeString(const std::string& str) = 0;
        virtual void close() = 0;

        virtual const std::string& getFilePath() const = 0;
        virtual const std::string& getFileName() const = 0;
        virtual const std::string& getFileExtension() const = 0;
    };
}  // namespace AnEngine

#endif
