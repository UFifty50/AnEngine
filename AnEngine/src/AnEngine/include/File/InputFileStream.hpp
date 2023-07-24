#ifndef INPUTFILESTREAM_HPP
#define INPUTFILESTREAM_HPP

#include <fstream>
#include <string>

#include "File/FileStream.hpp"


namespace AnEngine {
    class InputFileStream : private FileStream<Direction::Input> {
    public:
        InputFileStream(const std::string& path,
                        std::ios_base::openmode type = std::ios::binary);

        InputFileStream(nullptr_t);

        virtual ~InputFileStream() override;

        const std::string readAll() const;
        virtual void close() override;

        bool is_open() const { return this->stream.is_open(); }

        const std::string& getFilePath() const override { return this->path; }
        const std::string& getFileName() const override { return this->name; }
        const std::string& getFileExtension() const override { return this->extension; }

    private:
        std::string path;
        std::string name;
        std::string extension;

        std::ifstream stream;
    };
}  // namespace AnEngine

#endif
