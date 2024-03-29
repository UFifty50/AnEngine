#include "aepch.hpp"

#include "File/OutputFileStream.hpp"

#include <string>

#include "Core/Log.hpp"


namespace AnEngine {
    OutputFileStream::OutputFileStream(const std::string& path, std::ios_base::openmode type)
        : iMode(type) {
#if defined(AE_WIN)
        char exePathName[MAX_PATH];
        GetModuleFileNameA(NULL, exePathName, sizeof(exePathName));

        PathRemoveFileSpecA(exePathName);
        auto directory = std::string(exePathName);

        char combined[MAX_PATH];
        PathCombineA(combined, directory.c_str(), path.c_str());

        this->path = std::string(combined);
#elif defined(AE_LINUX)
        char exePathName[PATH_MAX];
        realpath("/proc/self/exe", exePathName);

        char* exePath = new char[std::strlen(exePathName) + 1];
        std::strncpy(exePath, exePathName, sizeof(exePath));

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

        stream.open(this->path, std::ios::out | type);
        if (!stream.is_open()) {
            AE_ERROR("Error opening file {0}", this->path);
        }
    }

    void OutputFileStream::writeString(const std::string& str) {
        if (!stream.is_open()) {
            AE_ERROR("File {0} is not open.", this->path);
            return;
        }
        stream.write(str.c_str(), str.size());
    }

    void OutputFileStream::close() {
        if (stream.is_open()) {
            stream.flush();
            stream.clear();
            stream.close();
        } else {
            AE_WARN("File {0} is already closed.", this->path);
        }
    }

    void OutputFileStream::operator=(OutputFileStream otherStream) {
        this->path = otherStream.path;
        this->name = otherStream.name;
        this->extension = otherStream.extension;
        this->iMode = otherStream.iMode;

        stream.open(this->path, std::ios::out | otherStream.iMode);
        if (!stream.is_open()) {
            AE_ERROR("Error opening file {0}", this->path);
        }

        otherStream.close();
    }
}  // namespace AnEngine
