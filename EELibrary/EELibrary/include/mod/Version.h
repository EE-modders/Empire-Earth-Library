#pragma once

#include "EELibrary.h"

namespace eelib {
	namespace mod
	{
		class EELIBRARY_API Version
		{
		public:
			Version(void);
			Version(const char* version);
			Version(const Version& version);
			Version(const int major, const int minor, const int patch);
			virtual ~Version(void);

			Version& operator=(const Version& version);
			bool operator==(const Version& version) const;
			bool operator>(const Version& version) const;
			bool operator<(const Version& version) const;
			bool operator>=(const Version& version) const;
			bool operator<=(const Version& version) const;
			bool operator!=(const Version& version) const;

			const char* GetString() const;
			void SetVersion(const char* version);

			void SetVersion(const int major, const int minor, const int patch);
			void SetMajor(const int major);
			void SetMinor(const int minor);
			void SetPatch(const int patch);

			int GetMajor() const;
			int GetMinor() const;
			int GetPatch() const;

		private:
			int _major;
			int _minor;
			int _patch;
		};
	}
}
