#pragma once

#include "EELibraryExports.h"

namespace eelib {
	namespace events
	{
		class EventPimpl
		{
		public:
			std::string name;
		};

		class EELIBRARY_API Event
		{
		public:
			// virtual event class for all events
			explicit Event(const char* name);
            virtual ~Event();

			bool operator==(const Event& event) const;
			bool operator!=(const Event& event) const;			
			
			const char* GetName() const;

		private:
			EventPimpl* _eventPimpl;
		};
	}
}
