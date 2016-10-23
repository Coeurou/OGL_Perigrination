#pragma once

#include <AntTweakBar.h>
#include "Event.hpp"
#include "EventManager.hpp"
#include "EventListener.hpp"

class ATBResource : public gs::IEventListener
{
public:
	ATBResource() 
	{
		gs::EventManager::GetInstance()->Subscribe(EventType::ET_KEY, this);
		gs::EventManager::GetInstance()->Subscribe(EventType::ET_MOUSE_MOVED, this);
		gs::EventManager::GetInstance()->Subscribe(EventType::ET_MOUSE_PRESSED, this);
		gs::EventManager::GetInstance()->Subscribe(EventType::ET_WINDOW_RESIZED, this);
	}

	~ATBResource() 
	{
		if (!gs::EventManager::IsInstanceDeleted()) {
			gs::EventManager::GetInstance()->Unsubscribe(EventType::ET_KEY, this);
			gs::EventManager::GetInstance()->Unsubscribe(EventType::ET_MOUSE_MOVED, this);
			gs::EventManager::GetInstance()->Unsubscribe(EventType::ET_MOUSE_PRESSED, this);
			gs::EventManager::GetInstance()->Unsubscribe(EventType::ET_WINDOW_RESIZED, this);
		}
	}

	void OnEvent(gs::Event e) override
	{
		switch (e.GetEventType())
		{
		case EventType::ET_KEY:
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
	
	void OnKeyPressed(const gs::Event& e) 
	{		
		if (e.args.keyState == 1) {
			TwEventKeyGLFW(e.args.key, 1);
		}
	}

	void OnMousePressed(const gs::Event& e) 
	{
		TwEventMouseButtonGLFW(e.args.mouseButton, e.args.mouseButtonState);
	}

	void OnMouseMoved(const gs::Event& e)
	{
		TwMouseMotion(int(e.args.mousePosX), int(e.args.mousePosY));
	}

	void OnWindowResized(const gs::Event& e) 
	{
		TwWindowSize(e.args.width, e.args.height);
	}
};
