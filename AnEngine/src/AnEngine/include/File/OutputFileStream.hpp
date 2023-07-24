#ifndef OUTPUTFILESTREAM_HPP
#define OUTPUTFILESTREAM_HPP

#include "File/FileStream.hpp"


namespace AnEngine {
    class OutputFileStream : private FileStream<Direction::Output> {
    public:
        OutputFileStream(const std::string& path,
                         std::ios_base::openmode type = std::ios::binary);

        OutputFileStream();

        virtual ~OutputFileStream();

        void writeString(const std::string& str);
        void operator=(OutputFileStream stream);

        template <typename T>
        OutputFileStream& operator<<(const T& data) {
            stream << data;
            return *this;
        }

        template <>
        OutputFileStream& operator<< <std::string>(const std::string& data) {
            this->operator<<(data.c_str());
            return *this;
        }

        bool is_open() const { return this->stream.is_open(); }

        virtual const std::string& getFilePath() const override { return this->path; }
        virtual const std::string& getFileName() const override { return this->name; }
        virtual const std::string& getFileExtension() const override {
            return this->extension;
        }

        virtual void close() override;
        void flush() { this->stream.flush(); }

    private:
        std::string path;
        std::string name;
        std::string extension;
        std::ios_base::openmode iMode;

        std::ofstream stream;
    };
}  // namespace AnEngine

#endif
