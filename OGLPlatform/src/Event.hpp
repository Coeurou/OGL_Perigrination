#pragma once

#include <map>

enum class EventType
{
	ET_KEY_PRESSED,
	ET_KEY_RELEASED,
	ET_MOUSE_MOVED,
	ET_MOUSE_PRESSED,
	ET_WINDOW_RESIZED
};

struct Variant
{
	enum Type
	{
		TYPE_INTEGER,
		TYPE_DOUBLE,
		TYPE_COUNT
	};
	Type type;
	union
	{
		int asInteger;
		double asDouble;
	};
};

namespace gs
{
	class Event
	{
	public:
		static const std::string BUTTON;
		static const std::string BUTTON_STATE;
		static const std::string MOUSEX;
		static const std::string MOUSEY;
		static const std::string KEY;
		static const std::string WIDTH;
		static const std::string HEIGHT;

		Event(EventType type);
		EventType GetEventType() const;
		void AddArgument(const std::string& key, Variant arg);
		Variant GetArgument(const std::string& key) const;

	private:
		EventType eventType;
		std::map<std::string, Variant> args;
	};
}