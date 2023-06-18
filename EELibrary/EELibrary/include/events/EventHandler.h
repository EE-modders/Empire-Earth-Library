#pragma once

#include "events/EventID.h"
#include "events/IEventListener.h"
#include <functional>
#include <unordered_map>
#include <vector>

namespace eelib {
namespace events {
    class EELIBRARY_API EventHandler {
    public:
        EventHandler();
        virtual ~EventHandler();

    private:
        using IEventListenerPtrList = std::vector<IEventListener*>;

    public:
        void unregisterAll();

        template <typename TEvent>
        inline void registerListener(IEventListener* listener)
        {
            static_assert(std::is_base_of<Event, TEvent>::value,
                "TEvent should be a derived class of Event");

            EventID id = EventIDGenerator<Event>::getEventId<TEvent>();
            _implem->m_Listeners[id].push_back(listener);
        }

        template <typename TEvent>
        inline void unregisterListener(IEventListener* listener)
        {
            static_assert(std::is_base_of<Event, TEvent>::value,
                "TEvent should be a derived class of Event");

            EventID id = EventIDGenerator<Event>::getEventId<TEvent>();
            IEventListenerPtrList& listeners = _implem->m_Listeners[id];

            for (auto iterator = listeners.begin(); iterator != listeners.end();
                 ++iterator) {
                if (*iterator == listener) {
                    listeners.erase(iterator);
                    break;
                }
            }
        }

        template <typename TEvent, typename... Args>
        inline void sendEvent(Args... args)
        {
            static_assert(std::is_base_of<Event, TEvent>::value,
                "TEvent should be a derived class of Event");

            TEvent event(args...);
            sendEvent(event);
        }

        template <typename TEvent>
        inline void sendEvent(TEvent& event)
        {
            static_assert(std::is_base_of<Event, TEvent>::value,
                "TEvent should be a derived class of Event");

            const_cast<TEvent&>(event).m_EventHandler = this;
            EventID id = EventIDGenerator<Event>::getEventId<TEvent>();
            if (_implem->m_Listeners.find(id) != _implem->m_Listeners.end()) {
                for (IEventListener* listener : _implem->m_Listeners.at(id)) {
                    listener->onEvent(event);
                }
            }
        }

        template <typename TEvent>
        inline unsigned int getNbOfListeners() const
        {
            static_assert(std::is_base_of<Event, TEvent>::value,
                "TEvent should be a derived class of Event");
            EventID id = EventIDGenerator<Event>::getEventId<TEvent>();

            if (_implem->m_Listeners.find(id) != _implem->m_Listeners.end()) {
                return _implem->m_Listeners.at(id).size();
            }

            return 0u;
        }

    private:
        struct Pimpl {
            std::unordered_map<EventID, IEventListenerPtrList> m_Listeners;
        };
        Pimpl* _implem;
    };

} // namespace events
} // namespace eelib