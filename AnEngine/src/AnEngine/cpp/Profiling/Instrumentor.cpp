#include "aepch.hpp"

#include "Profiling/Instrumentor.hpp"

#include <mutex>
#include <sstream>
#include <string>


namespace AnEngine {
void Instrumentor::beginSession(const std::string& name,
                                const std::string& filepath) {
    if (activeSession) endSession();
    activeSession = true;
    outputFile.open(filepath);
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
    std::lock_guard lock(_lock);
    std::stringstream output;

    if (profileCount++ > 0) outputFile.writeString(",");

    std::string name = result.Name;
    std::ranges::replace(name, '"', '\'');

    output << "{";
    output << "\"cat\":\"function\",";
    output << "\"dur\":" << (result.end - result.start) << ',';
    output << "\"name\":\"" << name << "\",";
    output << "\"ph\":\"X\",";
    output << "\"pid\":0,";
    output << "\"tid\":" << result.threadID << ",";
    output << "\"ts\":" << result.start;
    output << "}";

    outputFile.writeString(output.str());
}

void Instrumentor::writeHeader() { outputFile.writeString("{\"otherData\": {},\"traceEvents\":["); }

void Instrumentor::writeFooter() { outputFile.writeString("]}"); }
}; // namespace AnEngine
