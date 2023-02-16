#pragma once

#include <exception>
#include <string>

namespace eelib
{
    class CoreException : public std::exception
    {
    public:
        CoreException(const char* message)
            : _message(message) {}
        virtual ~CoreException() override = default;

    private:
		std::string _message;
		std::string _context = "CoreException";
    };
}