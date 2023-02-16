#pragma once

#include <exception>
#include <string>

namespace eelib::mod
{
    class ModException : public std::exception
    {
    public:
        ModException(const char* message)
            : _message(message) {}
        virtual ~ModException() override = default;

    private:
        std::string _message;
        std::string _context = "ModException";
    };
}