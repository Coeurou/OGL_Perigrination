#pragma once

#include <map>

enum class EventType
{
	ET_KEY,
	ET_MOUSE_MOVED,
	ET_MOUSE_PRESSED,
	ET_WINDOW_RESIZED,
	ET_COUNT
};

enum class MouseButton
{
	MB_LEFT,
	MB_RIGHT,
	MB_MIDDLE,
	MB_COUNT
};

enum class ButtonState
{
	BUTTON_RELEASED,
	BUTTON_PRESSED,
	BUTTON_REPEAT,
	BUTTON_STATE_COUNT
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