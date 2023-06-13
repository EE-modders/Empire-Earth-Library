#pragma once

#include <events/Event.h>
#include <assert.h>

namespace eelib {
	namespace events
	{

		class EELIBRARY_API IEventListener
		{
		public:
			virtual void onEvent(Event& event) = 0;
		};

		template<typename TEvent>
		class EventListener : public IEventListener
		{
			static_assert(std::is_base_of<Event, TEvent>::value, "TEvent should be a derived class of Event");

		public:
			virtual void onScoppedEvent(TEvent& event) = 0;

			void onEvent(Event& event) override
			{
				TEvent* castedEvent = dynamic_cast<TEvent*>(&event);
				static_assert(std::is_base_of<Event, TEvent>::value, "TEvent should be a derived class of Event");

				onScoppedEvent(*castedEvent);
			}
		};

		template class EELIBRARY_API EventListener<Event>;
	}
}
