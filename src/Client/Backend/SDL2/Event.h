#pragma once
#include "../Event.h"

namespace Backend
{
	namespace Event
	{
		class Event_SDL2 : public Event
		{
			public:
				//Constructors and destructor
				Event_SDL2();
				Event_SDL2(const Config config);
				~Event_SDL2() override;
				
				//Event interface
				bool SetConfig(const Config config) override;
				
				bool PollEvent(EventData &event_data) override;
		};
	}
}
