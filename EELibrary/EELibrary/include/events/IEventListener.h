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

		public:
			virtual void onScoppedEvent(TEvent& event) = 0;

			void onEvent(Event& event) override
			{
				static_assert(std::is_base_of<Event, TEvent>::value, "TEvent should be a derived class of Event");
				onScoppedEvent(static_cast<TEvent&>(event));
			}
		};

		template class EELIBRARY_API EventListener<Event>;
	}
}
