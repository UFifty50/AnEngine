#ifndef INSTRUMENTOR_HPP
#define INSTRUMENTOR_HPP

#include <mutex>
#include <string>

#include "File/OutputFileStream.hpp"


namespace AnEngine {
    struct ProfileResult {
        std::string Name;
        long long start;
        long long end;
        uint32_t threadID;
    };

    class Instrumentor {
    public:
        ~Instrumentor();
        void beginSession(const std::string& name,
                          const std::string& filepath = "results.json");
        void endSession();
        void writeProfile(const ProfileResult& result);

        void writeHeader();
        void writeFooter();

        static Instrumentor& Get() {
            static Instrumentor instance;
            return instance;
        }

    private:
        Instrumentor();


        // state
        std::string sessionName = "None";
        OutputFileStream outputFile;
        int profileCount;
        bool activeSession;
        std::mutex _lock;
    };
};  // namespace AnEngine

#endif
