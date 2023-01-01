#pragma once

#include "EELibrary.h"

namespace eelib
{
	namespace memory
	{
		enum GameType
		{
			Unknown, EE, AoC
		};
		
		class EELIBRARY_API Memory
		{
		public:
			GameType Initialize();
			void Uninitialize();
			
			int HookFunction(LPVOID hookAddress, LPVOID hookFunction, LPVOID* originalFunction = NULL);
			int UnhookFunction(LPVOID hookAddress);

		private:
			void InitializeEE();
			void InitializeAoC();
			
		// Module Addresses
		public:
			DWORD gameAddress;
			DWORD lleAddress;

		// Public addresses
		public:
			LPVOID mainHook;
			LPVOID mainHookOrg;
		
		// Private addresses
		private:
			CONST DWORD clientCodeAddress = 0x00001000;

		private:
			GameType _gameType;
		};
	}
}
