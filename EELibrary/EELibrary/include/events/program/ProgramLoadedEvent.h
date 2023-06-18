#pragma once

#include "events/Event.h"

namespace eelib {
namespace events {

    /**
     * @brief Event fired when the program is fully loaded
     */
    class EELIBRARY_API ProgramLoadedEvent : public Event {
    public:
        ProgramLoadedEvent()
            : Event()
        {
        }
        virtual ~ProgramLoadedEvent() { }
    };
}
}
