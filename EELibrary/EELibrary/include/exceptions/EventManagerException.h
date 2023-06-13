#pragma once

#include <exception>
#include <string>

namespace eelib::events
{
    class EventManagerException : public std::exception
    {
    public:
        EventManagerException(const char* message)
            : _message(message) {}
        virtual ~EventManagerException() override = default;

    private:
        std::string _message;
        std::string _context = "Event Manager";
    };
}