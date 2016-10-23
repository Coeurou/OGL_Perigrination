#pragma once

#include "Event.hpp"

namespace gs
{
	class IEventListener
	{
	public:
		IEventListener() {}
		virtual ~IEventListener() {}
		virtual void OnEvent(Event e) = 0;
	};
}