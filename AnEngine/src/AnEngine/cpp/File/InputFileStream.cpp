#include "aepch.hpp"

#include "File/InputFileStream.hpp"

namespace AnEngine {
    InputFileStream::InputFileStream(const std::string& path) {
#if defined(AE_WIN)
        char exePathName[MAX_PATH];
        GetModuleFileNameA(NULL, exePathName, sizeof(exePathName));

        char* exePath = new char[std::string(exePathName).length()+1];
        strcpy(exePath, exePathName);

        PathRemoveFileSpecA(exePath);
        auto directory = std::string(exePath);

        delete[] exePath;

        char combined[MAX_PATH];
        PathCombineA(combined, directory.c_str(), path.c_str());

        this->path = std::string(combined);
#elif defined(AE_LINUX)
        char exePathName[PATH_MAX];
        realpath("/proc/self/exe", exePathName);

        char* exePath = new char[std::string(exePathName).length() + 1];
        strcpy(exePath, exePathName);

        auto directory = std::string(dirname(exePath));

        delete[] exePath;

        this->path = directory + "/" + path;
#endif
        // winows and linux paths
        if (this->path.find_last_of("\\") != std::string::npos) {
            this->name = this->path.substr(this->path.find_last_of("\\") + 1);
        } else {         
            this->name = this->path.substr(this->path.find_last_of("/") + 1);
        }
        
        this->extension = this->path.substr(this->path.find_last_of(".") + 1);

        this->open(this->path);
        if (!this->is_open()) {
            perror(("Error opening file " + this->path).c_str());
        }
    }

    InputFileStream::~InputFileStream() {
        this->close();
    }

    const std::string InputFileStream::readAll() const {
        std::stringstream ss;
        ss << this->rdbuf();
        return ss.str();
    }
};
