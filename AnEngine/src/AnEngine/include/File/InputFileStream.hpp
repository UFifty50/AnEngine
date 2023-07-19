#ifndef INPUTFILESTREAM_HPP
#define INPUTFILESTREAM_HPP

#include <fstream>
#include <string>


namespace AnEngine {
    class InputFileStream : private std::ifstream {
    public:
        InputFileStream(const std::string& path,
                        ios_base::openmode type = std::ios::binary);

        ~InputFileStream();

        const std::string readAll() const;

        inline const std::string& getFilePath() const { return this->path; }
        inline const std::string& getFileName() const { return this->name; }
        inline const std::string& getFileExtension() const { return this->extension; }

        using std::ifstream::close;
        using std::ifstream::is_open;

    private:
        std::string path;
        std::string name;
        std::string extension;
    };
}  // namespace AnEngine

#endif
