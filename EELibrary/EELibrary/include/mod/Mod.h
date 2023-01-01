#pragma once

#include "pch.h"
#include "EELibrary.h"
#include "events/EventManager.h"
#include "mod/Version.h"

namespace eelib
{
	namespace mod
	{

		class ModPimpl
		{
		public:
			std::wstring _path;
			std::string _name;
		};

		class EELIBRARY_API Mod
		{
		public:
			Mod(char* name);
			virtual ~Mod(void);

			const char* GetName() const;
			const wchar_t* GetPath() const;
			const Version& GetVersion() const;
			bool IsEnabled() const;
			eelib::events::EventManager& GetEvent();

			// Mod: Called when the plugin is loaded.
			// Game: Called when the game is loading.
			// Return true to allow plugin to be enabled.
			// Blocking call.
			virtual bool OnInit() = 0;

			// Mod: Called when the plugin is enabled
			// Game: Called when the game is starting
			// Return true to allow plugin to be started
			// Non-blocking call.
			virtual bool OnStart() = 0;

			// Mod: Called when the plugin is unloaded.
			// Game (not always): Called when the game is unloading.
			// Return true when the plugin is disabled successfully.
			virtual bool OnStop() = 0;

		protected:
			friend class ModManager; // Allow ModManager to access to protected methods
			void SetPath(const wchar_t* name);
			void SetVersion(const Version& version);
			void SetVersion(const int major, const int minor, const int patch);
			void SetVersion(const char* version);
			void SetEnabled(bool enabled);

		private:
			ModPimpl* m_Implementation;
			bool _enabled;
			Version _version;
			eelib::events::EventManager _event;
		};
	}
}

