#pragma once

#include <exception>
#include <string>

namespace eelib::mod
{
    class ModManagerException : public std::exception
    {
    public:
        ModManagerException(const char* message)
            : _message(message) {}
        virtual ~ModManagerException() override = default;

    private:
        std::string _message;
        std::string _context = "ModException";
    };
}