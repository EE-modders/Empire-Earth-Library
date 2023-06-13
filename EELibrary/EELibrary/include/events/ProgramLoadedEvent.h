#pragma once

#include "events/Event.h"
#include <events/EventHandler.h>

namespace eelib {
	namespace events
	{
		class EELIBRARY_API ProgramLoadedEvent : public Event
		{
			// Args
			public:
				ProgramLoadedEvent(const char* programName, const char* programPath) : Event(), _programName(programName), _programPath(programPath) {}
				virtual ~ProgramLoadedEvent() {}

				// Getters
				const char* GetProgramName() const { return _programName; }
				const char* GetProgramPath() const { return _programPath; }

				// Setters
				void SetProgramName(const char* programName) { _programName = programName; }
				void SetProgramPath(const char* programPath) { _programPath = programPath; }

				private:
					const char* _programName;
					const char* _programPath;
		};
	}
}
