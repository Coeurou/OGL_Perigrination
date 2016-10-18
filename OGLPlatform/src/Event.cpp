#include "Event.hpp"

const std::string gs::Event::BUTTON = "BUTTON";
const std::string gs::Event::BUTTON_STATE = "BUTTON_STATE";
const std::string gs::Event::MOUSEX = "MOUSEX";
const std::string gs::Event::MOUSEY = "MOUSEY";
const std::string gs::Event::KEY = "KEY";
const std::string gs::Event::WIDTH = "WIDTH";
const std::string gs::Event::HEIGHT = "HEIGHT";

namespace gs
{
	Event::Event(EventType type) : eventType(type)
	{}

	EventType Event::GetEventType() const
	{
		return eventType;
	}

	void Event::AddArgument(const std::string& key, Variant arg)
	{
		args[key] = arg;
	}
	Variant Event::GetArgument(const std::string & key) const
	{
		return args.at(key);
	}
}