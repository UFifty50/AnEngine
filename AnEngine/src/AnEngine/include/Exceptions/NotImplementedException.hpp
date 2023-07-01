#ifndef NOTIMPLEMENTEDEXCEPTION_HPP
#define NOTIMPLEMENTEDEXCEPTION_HPP

#include <stdexcept>


namespace AnEngine {
    class NotImplementedException : private std::exception {
    public:
        NotImplementedException() : std::exception("Not implemented yet") {}
        NotImplementedException(const char* msg) : std::exception(msg) {}
        NotImplementedException(const std::string& msg) : std::exception(msg.c_str()) {}
    };
};

#endif // NOTIMPLEMENTEDEXCEPTION_HPP
