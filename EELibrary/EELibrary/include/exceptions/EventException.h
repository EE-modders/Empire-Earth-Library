#pragma once

#include <exception>
#include <string>

namespace eelib::events
{
    class EventException : public std::exception
    {
    public:
        EventException(const char* message)
            : _message(message) {}
        virtual ~EventException() override = default;

    private:
        std::string _message;
        std::string _context = "Event";
    };
}