#pragma once

#include "events/Event.h"
#include <events/EventHandler.h>

namespace eelib {
	namespace events
	{
		class EELIBRARY_API ProgramLoadedEvent : public CancelableEvent
		{
			// Args
			public:
				ProgramLoadedEvent(const int number, const char* string) : CancelableEvent(), _number(number), _string(const_cast<char*>(string)) {}
				virtual ~ProgramLoadedEvent() {}

				int getNumber() const
				{
					return _number;
				}

				char* getString() const
				{
					return _string;
				}

				void setNumber(const int number)
				{
					_number = number;
				}

				void setString(const char* string)
				{
					_string = const_cast<char*>(string);
				}

				private:
					int _number;
					char* _string;
		};
	}
}
