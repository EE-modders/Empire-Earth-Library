#include "pch.h"

#include <mod/Mod.h>

using namespace eelib::mod;

Mod::Mod(const char* name)
{
    _implem = new Pimpl();
    _implem->_name = name;
}

Mod::~Mod()
{
    delete _implem;
}

const char* Mod::GetName() const
{
    return _implem->_name.c_str();
}

const char* Mod::GetPath() const
{
    return _implem->_path.c_str();
}

const Version& Mod::GetVersion() const
{
    return _version;
}

void Mod::SetPath(const char* path)
{
    _implem->_path = path;
}

void Mod::SetVersion(const Version& version)
{
    _version = version;
}

void Mod::SetVersion(int major, int minor, int patch)
{
    _version = Version(major, minor, patch);
}

void Mod::SetVersion(const char* version)
{
    _version = Version(version);
}

bool Mod::IsRunning() const
{
    return _running;
}

bool Mod::IsInitialized() const
{
    return _initialized;
}

void Mod::SetRunning(bool running)
{
    _running = running;
}

void Mod::SetInitialized(bool initialized)
{
    _initialized = initialized;
}
