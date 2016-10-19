#pragma once

#include <map>

enum class EventType
{
	ET_KEY,
	ET_MOUSE_MOVED,
	ET_MOUSE_PRESSED,
	ET_WINDOW_RESIZED
};

struct Variant
{
	union
	{
		struct
		{
			double mousePosX;
			double mousePosY;
			int mouseButton;
			int mouseButtonState;
		};
		struct
		{
			int key;
			int keyState;
		};
		struct
		{
			int width;
			int height;
		};
	};
};

namespace gs
{
	class Event
	{
	public:
		Event(EventType type);
		EventType GetEventType() const;

		Variant args;
	private:
		EventType eventType;
	};
}