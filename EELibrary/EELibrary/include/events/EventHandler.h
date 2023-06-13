#pragma once
#include <unordered_map>
#include <vector>
#include <functional>

#include <events/EventID.h>
#include <events/IEventListener.h>

namespace eelib {
	namespace events
	{

		class EventHandlerPimpl
		{
		public:
			using IEventListenerPtrList = std::vector<IEventListener*>;
			std::unordered_map<EventID, IEventListenerPtrList> m_Listeners;
		};

		class EELIBRARY_API EventHandler
		{
		private:
			EventHandlerPimpl m_Pimpl = EventHandlerPimpl();
			using IEventListenerPtrList = std::vector<IEventListener*>;

		public:
			template<typename TEvent>
			inline void registerListener(IEventListener* listener)
			{
				static_assert(std::is_base_of<Event, TEvent>::value, "TEvent should be a derived class of Event");

				EventID id = EventIDGenerator<Event>::getEventId<TEvent>();
				m_Pimpl.m_Listeners[id].push_back(listener);
			}

			template<typename TEvent>
			inline void unregisterListener(IEventListener* listener)
			{
				static_assert(std::is_base_of<Event, TEvent>::value, "TEvent should be a derived class of Event");

				EventID id = EventIDGenerator<Event>::getEventId<TEvent>();
				IEventListenerPtrList& listeners = m_Pimpl.m_Listeners[id];

				for (auto iterator = listeners.begin(); iterator != listeners.end(); ++iterator)
				{
					if (*iterator == listener)
					{
						listeners.erase(iterator);
						break;
					}
				}
			}

			template<typename TEvent, typename ...Args>
			inline void sendEvent(Args ...args)
			{
				static_assert(std::is_base_of<Event, TEvent>::value, "TEvent should be a derived class of Event");

				TEvent event(args...);
				sendEvent(event);
			}

			template<typename TEvent>
			inline void sendEvent(TEvent& event)
			{
				static_assert(std::is_base_of<Event, TEvent>::value, "TEvent should be a derived class of Event");

				const_cast<TEvent&>(event).m_EventHandler = this;
				EventID id = EventIDGenerator<Event>::getEventId<TEvent>();
				if (m_Pimpl.m_Listeners.find(id) != m_Pimpl.m_Listeners.end())
				{
					for (IEventListener* listener : m_Pimpl.m_Listeners.at(id))
					{
						listener->onEvent(event);
					}
				}
			}

			template<typename TEvent>
			inline unsigned int getNbOfListeners() const
			{
				static_assert(std::is_base_of<Event, TEvent>::value, "TEvent should be a derived class of Event");
				EventID id = EventIDGenerator<Event>::getEventId<TEvent>();

				if (m_Pimpl.m_Listeners.find(id) != m_Pimpl.m_Listeners.end())
				{
					return m_Pimpl.m_Listeners.at(id).size();
				}

				return 0u;
			}

			void unregisterAll()
			{
				m_Pimpl.m_Listeners.clear();
			}
		};

	}
}