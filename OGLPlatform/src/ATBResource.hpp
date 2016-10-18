#pragma once

#include <AntTweakBar.h>
#include "Event.hpp"
#include "EventManager.hpp"
#include "EventListener.hpp"

class ATBResource : public gs::EventListener
{
public:
	ATBResource() 
	{
		gs::EventManager::GetInstance()->Subscribe(EventType::ET_KEY_PRESSED, this);
		gs::EventManager::GetInstance()->Subscribe(EventType::ET_MOUSE_MOVED, this);
		gs::EventManager::GetInstance()->Subscribe(EventType::ET_MOUSE_PRESSED, this);
		gs::EventManager::GetInstance()->Subscribe(EventType::ET_WINDOW_RESIZED, this);
	}

	~ATBResource() 
	{
		if (!gs::EventManager::IsInstanceDeleted()) {
			gs::EventManager::GetInstance()->Unsubscribe(EventType::ET_KEY_PRESSED, this);
			gs::EventManager::GetInstance()->Unsubscribe(EventType::ET_MOUSE_MOVED, this);
			gs::EventManager::GetInstance()->Unsubscribe(EventType::ET_MOUSE_PRESSED, this);
			gs::EventManager::GetInstance()->Unsubscribe(EventType::ET_WINDOW_RESIZED, this);
		}
	}

	void OnEvent(gs::Event e) override
	{
		switch (e.GetEventType())
		{
		case EventType::ET_KEY_PRESSED:
			OnKeyPressed(e);
			break;
		case EventType::ET_MOUSE_MOVED:
			OnMouseMoved(e);
			break;
		case EventType::ET_MOUSE_PRESSED:
			OnMousePressed(e);
			break;
		case EventType::ET_WINDOW_RESIZED:
			OnWindowResized(e);
			break;
		default:
			break;
		}
	}

	bool InitATB() { return TwInit(TW_OPENGL_CORE, NULL) == 1; }
	
	void OnKeyPressed(const gs::Event& args) 
	{		
		auto var = args.GetArgument(gs::Event::KEY);
		TwEventKeyGLFW(var.asInteger, 1);
	}

	void OnMousePressed(const gs::Event& args) 
	{
		auto button = args.GetArgument(gs::Event::BUTTON);
		auto buttonState = args.GetArgument(gs::Event::BUTTON_STATE);
		TwEventMouseButtonGLFW(button.asInteger, buttonState.asInteger);
	}

	void OnMouseMoved(const gs::Event& args)
	{
		auto posX = args.GetArgument(gs::Event::MOUSEX);
		auto posY = args.GetArgument(gs::Event::MOUSEY);
		TwMouseMotion(int(posX.asDouble), int(posY.asDouble));
	}

	void OnWindowResized(const gs::Event& args) 
	{
		auto w = args.GetArgument(gs::Event::WIDTH);
		auto h = args.GetArgument(gs::Event::HEIGHT);
		TwWindowSize(w.asInteger, h.asInteger);
	}
};
