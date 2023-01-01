#pragma once

#include "pch.h"
#include "EELibrary.h"
#include "mod/Mod.h"
#include <thread>

namespace eelib {
	namespace mod
	{
		
		class ModManagerPimpl
		{
		public:
			typedef std::map<std::wstring, Mod*> ModMap;
			typedef std::map<std::wstring, HMODULE > LibraryMap;
			typedef std::map<std::wstring, std::thread> ThreadMap;
			
			ModMap m_Mods;
			LibraryMap m_Libs;
			ThreadMap m_Threads;
		};

		class EELIBRARY_API ModManager
		{
		public:
			typedef Mod* (*fnCreateMod)(void);
			typedef void (*fnDestroyMod)(void);
			
			static ModManager& Instance();

			Mod* LoadMod(const wchar_t* modPath);
			bool IsModLoaded(const wchar_t* modPath) const;
			void UnloadMod(Mod*& plugin);

			void InitMod(Mod* mod);
			void StartMod(Mod* mod);
			void StopMod(Mod* mod);

		private:
			ModManager(void);
			~ModManager(void);

			ModManagerPimpl* _modManagerPimpl;
		};

	}
}
