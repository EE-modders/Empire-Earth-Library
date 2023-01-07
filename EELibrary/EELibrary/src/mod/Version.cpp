#include "pch.h"
#include "mod/Version.h"

using namespace eelib::mod;

Version::Version()
{
	_major = 0;
	_minor = 0;
	_patch = 0;
}

Version::Version(const char* version)
{
	SetVersion(version);
}

Version::Version(const Version& version)
{
	_major = version._major;
	_minor = version._minor;
	_patch = version._patch;
}

Version::Version(int major, int minor, int patch)
{
	_major = major;
	_minor = minor;
	_patch = patch;
}

Version::~Version()
{
}

Version& Version::operator=(const Version& version)
{
	_major = version._major;
	_minor = version._minor;
	_patch = version._patch;
	return *this;
}

bool Version::operator==(const Version& version) const
{
	return _major == version._major && _minor == version._minor && _patch == version._patch;
}

bool Version::operator>(const Version& version) const
{
	return _major > version._major ||
		(_major == version._major && _minor > version._minor) ||
		(_major == version._major && _minor == version._minor && _patch > version._patch);
}

bool Version::operator<(const Version& version) const
{
	return !(version > *this);
}

bool Version::operator>=(const Version& version) const
{
	return *this > version || *this == version;
}

bool Version::operator<=(const Version& version) const
{
	return *this < version || *this == version;
}

bool Version::operator!=(const Version& version) const
{
	return !(*this == version);
}

const char* Version::GetString() const
{
	char* version = new char[32];
	sprintf_s(version, 32, "%d.%d.%d", _major, _minor, _patch);
	return version;
}

void Version::SetVersion(const char* version)
{
	int major = 0, minor = 0, patch = 0;
	sscanf_s(version, "%d.%d.%d", &major, &minor, &patch);
	SetVersion(major, minor, patch);
}

void Version::SetVersion(int major, int minor, int patch)
{
	_major = major;
	_minor = minor;
	_patch = patch;
}

void Version::SetMajor(int major)
{
	_major = major;
}

void Version::SetMinor(int minor)
{
	_minor = minor;
}

void Version::SetPatch(int patch)
{
	_patch = patch;
}

int Version::GetMajor() const
{
	return _major;
}

int Version::GetMinor() const
{
	return _minor;
}

int Version::GetPatch() const
{
	return _patch;
}
