#pragma once

#include "pch.h"
#include "events/Event.h"

namespace eelib {
	namespace events
	{
		class EventManagerPimpl;
		
		class EELIBRARY_API EventManager
		{
		public:
			EventManager(void);
			~EventManager(void);
		
			typedef void (*EventCallback)(Event* event);
			
			void RegisterEventCallback(const Event& event, EventCallback callback);
			void UnregisterEventCallback(const Event& event, EventCallback callback);
			void UnregisterEventCallbacks(const Event& event);
			void UnregisterAllEventCallbacks();
			void FireEvent(Event* event);
			
		private:
			EventManagerPimpl* _eventCallbackPimpl;
		};
		
		class EventManagerPimpl
		{
		public:
			std::map<std::string, std::vector<EventManager::EventCallback>> eventCallbacks;
		};
	}
}