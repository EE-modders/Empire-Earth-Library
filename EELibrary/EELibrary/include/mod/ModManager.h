#pragma once

#include "pch.h"
#include "EELibraryExports.h"
#include "mod/Mod.h"
#include <thread>

namespace eelib
{
	namespace mod
	{
		
		class ModManagerPimpl
		{
		public:
			typedef std::map<std::string, Mod*> ModMap;
			typedef std::map<std::string, HMODULE> LibraryMap;
			typedef std::map<std::string, HANDLE> ThreadMap;
			
			ModMap m_Mods;
			LibraryMap m_Libs;
			ThreadMap m_Threads;
		};

		class EELIBRARY_API ModManager
		{
		public:
			ModManager();
            virtual ~ModManager();

			Mod* LoadMod(const std::string& modPath);
			void UnloadMod(Mod* plugin);
			void UnloadAllMods();

			bool IsModLoaded(const char* modPath) const;

			void InitMod(Mod* mod);
			void StartMod(Mod* mod);
			void StopMod(Mod* mod);

		private:
			ModManagerPimpl* _modManagerPimpl;
		};

	}
}
