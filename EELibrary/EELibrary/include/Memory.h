#pragma once

#include "EELibraryExports.h"

#define DEFINE_HOOK(fnReturnType, fnCallType, fnName, ...) \
	typedef fnReturnType(fnCallType fnName##Hook)(__VA_ARGS__); \
	fnName##Hook fnName##Org = NULL; \
    LPVOID fnName##Addr = NULL; \

#define REGISTER_HOOK_ADDR(fnName, addr) \
	fnName##Addr = reinterpret_cast<LPVOID>(addr); \

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
			
			// [[deprecated("Use HookFunction instead")]]
			int HookFunctionByApi(LPCWSTR pszModule, LPCSTR pszProcName, LPVOID pDetour, LPVOID* ppOriginal);

			bool IsExecutableAddress(LPVOID pAddress);

		private:
			void InitializeEE();
			void InitializeAoC();
			
		// Module Addresses
		public:
			DWORD moduleGameAddress;
			DWORD moduleLLEAddress;
			DWORD moduleTCPProtoAddress;
			DWORD moduleDX7DispAddress;
			DWORD moduleDX7TnLDispAddress;

		// Public addresses
		// MODULE_AnyName (+Addr for address, +Fn for function)
		public:
			DEFINE_HOOK(void, __fastcall*, Game_Start)
			DEFINE_HOOK(void, __cdecl*, LLE_UShutdown, bool coUninitialize, bool exit)
			
		
		// Private addresses
		private:
			CONST DWORD clientCodeAddress = 0x00001000;

		// Constants
		private:
			CONST BYTE _clientCodeEE = 0xE8;
			CONST BYTE _clientCodeAoC = 0x55;

		private:
			GameType _gameType;
		};
	}
}
