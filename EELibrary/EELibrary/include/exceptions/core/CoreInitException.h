#pragma once

#include "exceptions/CoreException.h"

namespace eelib
{
    class CoreInitException : public CoreException
    {
    public:
        CoreInitException(std::string msg, Logger::Level level = Logger::CRITICAL)
			: CoreException(msg, level) {}
        virtual ~CoreInitException() override = default;
    };
}