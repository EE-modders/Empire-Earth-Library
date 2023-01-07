#pragma once

#include "Logger.h"
#include <exception>

namespace eelib
{
    class CoreException : public std::exception
    {
    public:
        CoreException(std::string message, Logger::Level level = Logger::Level::CRITICAL)
            : _message(message), _level(level) {}
        virtual ~CoreException() override = default;

    private:
		std::string _message;
		std::string _context = "CoreException";
		Logger::Level _level;
    };
}