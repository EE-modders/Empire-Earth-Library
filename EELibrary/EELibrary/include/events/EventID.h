#pragma once

#include <cstddef>
#include <type_traits>
#include <events/Event.h>

namespace eelib {
	namespace events
	{

		typedef std::size_t EventID;

		template <typename TEventBase>
		class EventIDGenerator
		{
			static_assert(std::is_base_of<Event, TEventBase>::value, "TEvent should be a derived class of Event");

		public:
			template <typename TEvent>
			static EventID getEventId();

		private:
			static EventID m_nextEventId;
		};

		template <typename TEventBase>
		EventID EventIDGenerator<TEventBase>::m_nextEventId = 0;

		template<typename TEventBase>
		template<typename TEvent>
		inline EventID EventIDGenerator<TEventBase>::getEventId()
		{
			static const EventID id = m_nextEventId++;
			return id;
		}
	}
}