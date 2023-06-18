#include "pch.h"

#include "ModTemplate.h"
#include <iostream>

MOD_NAME::MOD_NAME(void)
    : eelib::mod::Mod(MOD_NAME_STRING)
{
    SetVersion(MOD_VERSION_MAJOR, MOD_VERSION_MINOR, MOD_VERSION_PATCH);
}

MOD_NAME::~MOD_NAME(void)
{
}

class EventListenerExample : public eelib::events::EventListener<eelib::events::ProgramLoadedEvent> {
public:
    virtual void onScoppedEvent(eelib::events::ProgramLoadedEvent& programLoadedEvent) override
    {
        /*std::cout << "Program loaded: " << programLoadedEvent.getNumber() << " | " << programLoadedEvent.getString() << " | " << programLoadedEvent.isCanceled() << std::endl;
        programLoadedEvent.setNumber(42);
        programLoadedEvent.setString("Hello World!");
        programLoadedEvent.setCanceled(true);
        std::cout << "Program loaded: " << programLoadedEvent.getNumber() << " | " << programLoadedEvent.getString() << " | " << programLoadedEvent.isCanceled() << std::endl;*/
        std::cout << "Program loaded" << std::endl;
    }
};

bool MOD_NAME::OnStart()
{
    OnProgramLoaded.registerListener<eelib::events::ProgramLoadedEvent>(new EventListenerExample());
    return true;
}

bool MOD_NAME::OnUpdate()
{
    while (IsRunning()) {
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
