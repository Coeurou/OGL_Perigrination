#include "Event.hpp"

namespace gs
{
	Event::Event(EventType type) : eventType(type)
	{}

	EventType Event::GetEventType() const
	{
		return eventType;
	}
}