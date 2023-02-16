#pragma once

#include "EELibraryExports.h"

namespace eelib {
	namespace mod
	{
		class EELIBRARY_API Version
		{
		public:
			Version();
			explicit Version(const char* version);
			Version(const Version& version);
			Version(int major, int minor, int patch);
			virtual ~Version();

			Version& operator=(const Version& version);
			bool operator==(const Version& version) const;
			bool operator>(const Version& version) const;
			bool operator<(const Version& version) const;
			bool operator>=(const Version& version) const;
			bool operator<=(const Version& version) const;
			bool operator!=(const Version& version) const;

			const char* GetString() const;
			void SetVersion(const char* version);

			void SetVersion(int major, int minor, int patch);
			void SetMajor(int major);
			void SetMinor(int minor);
			void SetPatch(int patch);

			int GetMajor() const;
			int GetMinor() const;
			int GetPatch() const;

		private:
			int _major;
			int _minor;
			int _patch;
			char _versionStr[16];
		};
	}
}
