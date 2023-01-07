#pragma once

#include "EELibraryExports.h"


#define DEFINE_HOOK(fnReturnType, fnCallType, fnName, ...) \
	typedef fnReturnType(fnCallType fnName##Hook)(__VA_ARGS__); \
	fnName##Hook fnName##Org = NULL; \
    LPVOID fnName##Addr = NULL; \

#define REGISTER_HOOK_ADDR(fnName, addr) \
	fnName##Addr = (LPVOID)(addr); \

#define LINK_HOOK(fnReturnType, fnName, ...) \
	fnReturnType fnName##HookFn(__VA_ARGS__)

namespace eelib
{
	namespace memory
	{
		enum GameType
		{
			EE, AoC
		};

		
		class Memory
		{
		public:
			Memory() = default;
            virtual ~Memory() = default;

			void Initialize();
			void Uninitialize();
			
			int HookFunction(LPVOID orgAddress, LPVOID hookFn, LPVOID* orgFn = nullptr);
			int UnhookFunction(LPVOID orgAddress);

			bool IsExecutableAddress(LPVOID pAddress);

		private:
			void InitializeEE();
			void InitializeAoC();
			
		// Module Addresses
		public:
			DWORD gameAddress;
			DWORD lleAddress;

		// Public addresses
		// BIN_AnyName (+Addr for address, +Fn for function)
		public:
			DEFINE_HOOK(void, __fastcall*, Game_Start, int regecx)
			DEFINE_HOOK(void, __cdecl*, LLE_UShutdown, bool coUninitialize, bool exit)
			
		
		// Private addresses
		private:
			CONST DWORD clientCodeAddress = 0x00001000;

		// Constants
		private:
			CONST BYTE clientCodeEE = 0xE8;
			CONST BYTE clientCodeAoC = 0x55;

		private:
			GameType _gameType;
		};
	}
}
