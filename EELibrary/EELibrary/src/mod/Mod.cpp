#include "pch.h"
#include "mod/Mod.h"

using namespace eelib::mod;

Mod::Mod(char* name)
{
	m_Implementation = new ModPimpl();
	m_Implementation->_name = name;
}

Mod::~Mod(void)
{
	delete m_Implementation;
}

const char* Mod::GetName() const
{
	return m_Implementation->_name.c_str();
}

const wchar_t* Mod::GetPath() const
{
	return m_Implementation->_path.c_str();
}

const Version& Mod::GetVersion() const
{
	return _version;
}

void Mod::SetPath(const wchar_t* path)
{
	m_Implementation->_path = path;
}

void Mod::SetVersion(const Version& version)
{
	_version = version;
}

void Mod::SetVersion(const int major, const int minor, const int patch)
{
	_version = Version(major, minor, patch);
}

void Mod::SetVersion(const char* version)
{
	_version = Version(version);
}

bool Mod::IsEnabled() const
{
	return _enabled;
}

void Mod::SetEnabled(bool enabled)
{
	_enabled = enabled;
}

eelib::events::EventManager& Mod::GetEvent()
{
	return _event;
}
