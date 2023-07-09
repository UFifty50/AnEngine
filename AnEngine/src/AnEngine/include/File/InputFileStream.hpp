#ifndef INPUTFILESTREAM_HPP
#define INPUTFILESTREAM_HPP

#include <fstream>
#include <string>


namespace AnEngine {
    class InputFileStream : private std::ifstream {
    private:
        std::string path;
        std::string name;
        std::string extension;

    public:
        InputFileStream(const std::string& path);
        ~InputFileStream();

        const std::string readAll() const;

        inline const std::string& getFilePath() const { return this->path; }
        inline const std::string& getFileName() const { return this->name; }
        inline const std::string& getFileExtension() const {
            return this->extension;
        }

        using std::ifstream::close;
        using std::ifstream::is_open;
    };
}  // namespace AnEngine

#endif
