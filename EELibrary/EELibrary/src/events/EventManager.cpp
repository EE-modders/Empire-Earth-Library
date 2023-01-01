#include "pch.h"
#include "events/EventManager.h"

using namespace eelib::events;

EventManager::EventManager()
{
	_eventCallbackPimpl = new EventManagerPimpl();
}

EventManager::~EventManager()
{
	delete _eventCallbackPimpl;
}

void EventManager::RegisterEventCallback(const Event& event, EventCallback callback)
{
	_eventCallbackPimpl->eventCallbacks[event.GetName()].push_back(callback);
}

void EventManager::UnregisterEventCallback(const Event& event, EventCallback callback)
{
	auto& callbacks = _eventCallbackPimpl->eventCallbacks[event.GetName()];
	auto it = std::find(callbacks.begin(), callbacks.end(), callback);
	if (it != callbacks.end())
	{
		callbacks.erase(it);
	}
}

void EventManager::UnregisterEventCallbacks(const Event& event)
{
	_eventCallbackPimpl->eventCallbacks.erase(event.GetName());
}

void EventManager::UnregisterAllEventCallbacks()
{
	_eventCallbackPimpl->eventCallbacks.clear();
}

void EventManager::FireEvent(Event* event)
{
	auto& callbacks = _eventCallbackPimpl->eventCallbacks[event->GetName()];
	for (auto it = callbacks.begin(); it != callbacks.end(); ++it)
	{
		(*it)(event);
	}
}
