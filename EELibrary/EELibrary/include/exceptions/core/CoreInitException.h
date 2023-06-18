#pragma once

#include "exceptions/CoreException.h"

namespace eelib {
class CoreInitException : public CoreException {
public:
    CoreInitException(const char* msg)
        : CoreException(msg)
    {
    }
    virtual ~CoreInitException() override = default;
};
}