#pragma once

#include "exceptions/ModException.h"

namespace eelib::mod {
class ModInitException : public ModException {
public:
    ModInitException(const char* msg)
        : ModException(msg)
    {
    }
    virtual ~ModInitException() override = default;
};
}
