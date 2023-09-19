#include "aepch.hpp"

#include "Profiling/Instrumentor.hpp"

#include <mutex>
#include <string>


namespace AnEngine {
    void Instrumentor::beginSession(const std::string& name,
                                    const std::string& filepath) {
        if (activeSession) endSession();
        activeSession = true;
        outputFile = OutputFileStream(filepath);
        writeHeader();
        sessionName = name;
    }

    void Instrumentor::endSession() {
        if (!activeSession) return;
        activeSession = false;
        writeFooter();
        outputFile.close();
        profileCount = 0;
    }

    void Instrumentor::writeProfile(const ProfileResult& result) {
        std::lock_guard<std::mutex> lock(_lock);

        if (profileCount++ > 0) outputFile << ",";

        std::string name = result.Name;
        std::replace(name.begin(), name.end(), '"', '\'');

        outputFile << "{";
        outputFile << "\"cat\":\"function\",";
        outputFile << "\"dur\":" << (result.end - result.start) << ',';
        outputFile << "\"name\":\"" << name << "\",";
        outputFile << "\"ph\":\"X\",";
        outputFile << "\"pid\":0,";
        outputFile << "\"tid\":" << result.threadID << ",";
        outputFile << "\"ts\":" << result.start;
        outputFile << "}";
    }

    void Instrumentor::writeHeader() {
        outputFile << "{\"otherData\": {},\"traceEvents\":[";
    }

    void Instrumentor::writeFooter() { outputFile << "]}"; }
};  // namespace AnEngine
