#pragma once

#include "EELibraryExports.h"

namespace eelib {
	namespace events
	{
		class EventHandler;

		class EELIBRARY_API Event
		{
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
	}
}
