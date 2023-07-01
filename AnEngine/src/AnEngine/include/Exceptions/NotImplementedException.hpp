#ifndef NOTIMPLEMENTEDEXCEPTION_HPP
#define NOTIMPLEMENTEDEXCEPTION_HPP

#include <stdexcept>


namespace AnEngine {
    class NotImplementedException : public std::exception {
    public:
        const char* what() {
            return "Not implemented yet";
        }
    };
};

#endif // NOTIMPLEMENTEDEXCEPTION_HPP
