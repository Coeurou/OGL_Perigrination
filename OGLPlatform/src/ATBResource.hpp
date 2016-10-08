#pragma once

#include <AntTweakBar.h>
#include "EventManager.hpp"

class ATBResource
{
public:
	ATBResource() 
	{
		gs::EventFun keyEventFun(0, [=](const gs::EventArgs& args) {
			this->OnKeyPressed(args);
		});
		gs::EventManager::GetInstance().Subscribe(std::make_pair(gs::EventType::ET_KEY_PRESSED, keyEventFun));

		gs::EventFun mouseEventFun(0, [=](const gs::EventArgs& args) {
			this->OnMouseMoved(args);
		});
		gs::EventManager::GetInstance().Subscribe(std::make_pair(gs::EventType::ET_MOUSE_MOVED, mouseEventFun));

		gs::EventFun mousePressedEventFun(0, [=](const gs::EventArgs& args) {
			this->OnMousePressed(args);
		});
		gs::EventManager::GetInstance().Subscribe(std::make_pair(gs::EventType::ET_MOUSE_PRESSED, mousePressedEventFun));

		gs::EventFun resizeEventFun(0, [=](const gs::EventArgs& args) {
			this->OnWindowResized(args);
		});
		gs::EventManager::GetInstance().Subscribe(std::make_pair(gs::EventType::ET_WINDOW_RESIZED, resizeEventFun));
	}

	~ATBResource() 
	{
		gs::EventFun keyEventFun(0, [=](const gs::EventArgs& args) {
			this->OnKeyPressed(args);
		});
		gs::EventManager::GetInstance().Unsubscribe(std::make_pair(gs::EventType::ET_KEY_PRESSED, keyEventFun));

		gs::EventFun mouseEventFun(0, [=](const gs::EventArgs& args) {
			this->OnMouseMoved(args);
		});
		gs::EventManager::GetInstance().Unsubscribe(std::make_pair(gs::EventType::ET_MOUSE_MOVED, mouseEventFun));

		gs::EventFun mousePressedEventFun(0, [=](const gs::EventArgs& args) {
			this->OnMousePressed(args);
		});
		gs::EventManager::GetInstance().Unsubscribe(std::make_pair(gs::EventType::ET_MOUSE_PRESSED, mousePressedEventFun));

		gs::EventFun resizeEventFun(0, [=](const gs::EventArgs& args) {
			this->OnWindowResized(args);
		});
		gs::EventManager::GetInstance().Unsubscribe(std::make_pair(gs::EventType::ET_WINDOW_RESIZED, resizeEventFun));
		TwTerminate(); 
	}

	bool InitATB() { return TwInit(TW_OPENGL_CORE, NULL) == 1; }
	
	void OnKeyPressed(const gs::EventArgs& args) 
	{
		const auto& keyEvent = static_cast<const gs::KeyEventArgs&>(args);
		TwEventKeyGLFW(keyEvent.key, 1);
	}

	void OnMousePressed(const gs::EventArgs& args) 
	{
		const auto& mouseEvent = static_cast<const gs::MouseEventArgs&>(args);
		TwEventMouseButtonGLFW(mouseEvent.button, mouseEvent.state);
	}

	void OnMouseMoved(const gs::EventArgs& args)
	{
		const auto& mouseEvent = static_cast<const gs::MouseEventArgs&>(args);
		TwMouseMotion(int(mouseEvent.posX), int(mouseEvent.posY));
	}

	void OnWindowResized(const gs::EventArgs& args) 
	{
		const auto& resizeEvent = static_cast<const gs::ResizeEventArgs&>(args);
		TwWindowSize(resizeEvent.width, resizeEvent.height);
	}
};
