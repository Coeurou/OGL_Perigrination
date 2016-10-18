#pragma once

#include "Event.hpp"

namespace gs
{
	class EventListener
	{
	public:
		EventListener() {}
		virtual ~EventListener() {}

		virtual void OnEvent(Event e) = 0;
	};
}