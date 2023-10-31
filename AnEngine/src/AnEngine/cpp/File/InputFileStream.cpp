#include "aepch.hpp"

#include "File/InputFileStream.hpp"

namespace AnEngine {
    InputFileStream::InputFileStream(const std::string& path,
                                     std::ios_base::openmode type) {
#if defined(AE_WIN)
        char exePathName[MAX_PATH];
        GetModuleFileNameA(NULL, exePathName, sizeof(exePathName));

        char* exePath = new char[std::strlen(exePathName) + 1];
        std::strncpy(exePath, exePathName, sizeof(exePath));

        PathRemoveFileSpecA(exePath);
        auto directory = std::string(exePath);

        delete[] exePath;

        char combined[MAX_PATH];
        PathCombineA(combined, directory.c_str(), path.c_str());

        this->path = std::string(combined);
#elif defined(AE_LINUX)
        char exePathName[PATH_MAX];
        realpath("/proc/self/exe", exePathName);

        char* exePath = new char[std::strlen(exePathName) + 1];
        std::strncpy(exePath, exePathName, sizeof(exepath));

        auto directory = std::string(dirname(exePath));

        delete[] exePath;

        this->path = directory + "/" + path;
#endif
        // split path
        if (this->path.find_last_of("\\") != std::string::npos) {
            this->name = this->path.substr(this->path.find_last_of("\\") + 1);
        } else {
            this->name = this->path.substr(this->path.find_last_of("/") + 1);
        }

        this->extension = this->path.substr(this->path.find_last_of(".") + 1);

        stream.open(this->path, std::ios::in | type);
        if (!stream.is_open()) {
            perror(("Error opening file " + this->path).c_str());
        }
    }

    InputFileStream::InputFileStream(nullptr_t) {
        path = "<internal file>";
        name = "Internal File";
        extension = "";
    }

    InputFileStream::~InputFileStream() {
        if (stream.is_open()) this->close();
    }

    const std::string InputFileStream::readAll() const {
        std::stringstream ss;
        ss << stream.rdbuf();
        return ss.str();
    }
    void InputFileStream::close() {
        if (stream.is_open()) {
            stream.clear();
            stream.close();
        } else {
            AE_ERROR("File {0} is already closed.", this->path);
        }
    }
};  // namespace AnEngine
