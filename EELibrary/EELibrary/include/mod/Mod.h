#pragma once

#include "pch.h"
#include "EELibraryExports.h"
#include "events/EventManager.h"
#include "mod/Version.h"
#include "Memory.h"

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
			explicit Mod(const char* name);
            virtual ~Mod(void);

			const char* GetName() const;
			const wchar_t* GetPath() const;
			const Version& GetVersion() const;
			bool IsRunning() const;
			bool IsInitialized() const;
			eelib::events::EventManager& GetEvent();
			eelib::memory::Memory& GetMemory();

			typedef int (*fnGetLibraryVersion)(void);
			typedef Mod* (*fnCreateMod)(void);
			typedef void (*fnDestroyMod)(void);
			
			/*
				Called when the mod is starting up.
				This call block the game thread.
				This is the place to initialize your mod.
				Don't do any heavy work here!
				
				Return:
				1: Success, mod is initialized.
					OnUpdate() will be called.
				0: Failure, mod is not initialized.
					OnUpdate() will not be called.
			*/
			virtual bool OnStart() = 0;

			/*
				Called when the mod is running.
				This function is called from a separate thread.
				Do all your heavy work here :D
				Return:
				1: Everything is fine!
				0: Something went wrong!
			*/
			virtual bool OnUpdate() = 0;

			/*
				Called when the mod is shutting down.
				This call block the game thread.
				This is the place to clean up your mod.
				The game (or maybe Windows) will try to
				kill the thread after a certain amount of time.
				So... Again don't do any heavy work here :)
			*/
			virtual bool OnStop() = 0;

		protected:
			friend class ModManager; // Allow ModManager to access to protected methods
			void SetPath(const wchar_t* name);
			void SetVersion(const Version& version);
			void SetVersion(int major, int minor, int patch);
			void SetVersion(const char* version);
			void SetRunning(bool running);
			void SetInitialized(bool initialized);

		private:
			ModPimpl* m_Implementation;
			bool _running = false;
			bool _initialized = false;
			Version _version;
			eelib::events::EventManager _event;
		};
	}
}

