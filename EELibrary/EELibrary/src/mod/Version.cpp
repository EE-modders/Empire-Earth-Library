#include "pch.h"
#include "mod/Version.h"

using namespace eelib::mod;

Version::Version()
{
	SetVersion(0, 0, 0);
}

Version::~Version()
{
}

Version::Version(const char* version)
{
	SetVersion(version);
}

Version::Version(const Version& version)
{
	SetVersion(version._major, version._minor, version._patch);
}

Version::Version(int major, int minor, int patch)
{
	SetVersion(major, minor, patch);
}

Version& Version::operator=(const Version& version)
{
	SetVersion(version._major, version._minor, version._patch);
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
	return _versionStr;
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
	sprintf_s(_versionStr, 16, "%d.%d.%d", _major, _minor, _patch);
}

void Version::SetMajor(int major)
{
	SetVersion(major, _minor, _patch);
}

void Version::SetMinor(int minor)
{
	SetVersion(_major, minor, _patch);
}

void Version::SetPatch(int patch)
{
	SetVersion(_major, _minor, patch);
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
