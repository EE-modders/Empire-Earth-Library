#pragma once

#include "events/Event.h"

namespace eelib {
	namespace events
	{
		class ProgramLoadedEvent : Event
		{
		public:
			ProgramLoadedEvent();
			
		private:
			int test;
		};
	}
}
