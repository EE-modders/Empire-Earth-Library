#include "pch.h"
#include "events/Event.h"

using namespace eelib::events;

Event::Event(const char* name)
{
	_eventPimpl = new EventPimpl();
	_eventPimpl->name = name;
}

Event::~Event()
{
	delete _eventPimpl;
}

bool Event::operator==(const Event& event) const
{
	return _eventPimpl->name == event._eventPimpl->name;
}

bool Event::operator!=(const Event& event) const
{
	return _eventPimpl->name != event._eventPimpl->name;
}

const char* Event::GetName() const
{
	return _eventPimpl->name.c_str();
}