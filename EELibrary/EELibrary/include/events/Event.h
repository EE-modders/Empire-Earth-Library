#pragma once

#include "EELibraryExports.h"

namespace eelib {
namespace events {
    class EventHandler;

    class EELIBRARY_API Event {
    public:
        virtual ~Event() = default;

    public:
        EventHandler* getSender() const
        {
            return m_EventHandler;
        }

    private:
        EventHandler* m_EventHandler = nullptr;

        friend class EventHandler;
    };

    class EELIBRARY_API CancelableEvent : public Event {
    public:
        virtual ~CancelableEvent() = default;

    public:
        bool isCanceled() const
        {
            return _isCanceled;
        }

        void setCanceled(bool canceled)
        {
            _isCanceled = canceled;
        }

    private:
        bool _isCanceled = false;
    };

}
}
