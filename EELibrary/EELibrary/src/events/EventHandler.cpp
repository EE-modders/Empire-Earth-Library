#include "pch.h"

#include "events/EventHandler.h"

using namespace eelib::events;

EventHandler::EventHandler()
{
    _implem = new Pimpl();
}

EventHandler::~EventHandler()
{
    delete _implem;
}

void EventHandler::unregisterAll()
{
    _implem->m_Listeners.clear();
}