#include "pch.h"
#include "ModTemplate.h"

MOD_NAME::MOD_NAME(void) : eelib::mod::Mod(MOD_NAME_STRING)
{
	SetVersion(MOD_VERSION_MAJOR, MOD_VERSION_MINOR, MOD_VERSION_PATCH);
}

MOD_NAME::~MOD_NAME(void)
{
}

//EVENT_HANDLER(ProgramLoaded)
//printf("Program loaded: %s, %s\n", eventData.language, eventData.isAdmin ? "true" : "false");
//}

bool MOD_NAME::OnStart()
{
	/*std::cout << "ModTemplate::OnInit()" << std::endl;
	GetEvent().RegisterHandler(ModEvent::ProgramLoadedEvent, OnProgramLoadedEvent);*/
	return true;
}

bool MOD_NAME::OnUpdate()
{
	while (IsRunning())
	{
		/*
			Do stuff here, for most mods this will not be needed
			Because you will be using the event system to do stuff
			This loop must only be used if it can't be done in the event system
			Like for example if you want to detect a key press
		*/

		/*
			At any time, you can call SetRunning(false)
			This will stop the thread at the end of the current loop
		*/
		
		Sleep(MOD_UPDATE_INTERVAL);
	}
	/*std::cout << "ModTemplate::OnStart()" << std::endl;*/
	return true;
}

bool MOD_NAME::OnStop()
{
	/*std::cout << "ModTemplate::OnStop()" << std::endl;*/
	return true;
}
