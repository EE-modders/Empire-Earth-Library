#pragma once

#include "events/Event.h"

namespace eelib {
	namespace events
	{
		class ProgramLoadedEvent : Event
		{
		public:
			ProgramLoadedEvent(void);
			
		private:
			int test;
		};
	}
}
