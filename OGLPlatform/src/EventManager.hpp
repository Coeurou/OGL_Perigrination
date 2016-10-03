//
//  EventManager.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 14/08/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include "Uncopyable.hpp"
#include <map>
#include <vector>
#include <queue>
#include <functional>

namespace gs
{
    enum class EventType
    {
        ET_KEY_PRESSED,
        ET_MOUSE_MOVED,
        ET_MOUSE_PRESSED,
        ET_WINDOW_RESIZED
    };
    
    class EventArgs
    {
    public:
        EventArgs() {}        
        virtual ~EventArgs() {}
    };
    
    class MouseEventArgs : public EventArgs
    {
    public:
		MouseEventArgs() : button(0), state(0), posX(0), posY(0) {}
        MouseEventArgs(int b, int s, double pX, double pY)
            : button(b), state(s), posX(pX), posY(pY) {}
		MouseEventArgs(const MouseEventArgs& other)
			: button(other.button), state(other.state), posX(other.posX), posY(other.posY) {}
		MouseEventArgs(MouseEventArgs&& other) noexcept
			: button(std::move(other.button)), state(std::move(other.state)), posX(std::move(other.posX)), posY(std::move(other.posY)) {}
        virtual ~MouseEventArgs() {}

		MouseEventArgs& operator=(const MouseEventArgs& other)
		{
			button = other.button;
			state = other.state;
			posX = other.posX;
			posY = other.posY;
			return *this;
		}
        
		MouseEventArgs& operator=(MouseEventArgs&& other)
		{
			button = std::move(other.button);
			state = std::move(other.state);
			posX = std::move(other.posX);
			posY = std::move(other.posY);
			return *this;
		}

        int button;
        int state;
        double posX;
        double posY;
    };
    
    class KeyEventArgs : public EventArgs
    {
    public:
        KeyEventArgs(int k, int w, int h) : key(k), width(w), height(h) {}
		KeyEventArgs(KeyEventArgs&& other) noexcept
			: key(std::move(other.key)), width(std::move(other.width)), height(std::move(other.height)) {}
        ~KeyEventArgs() {}
    
        int key;
        int width;
        int height;
    };
    
    class ResizeEventArgs : public EventArgs
    {
    public:
        ResizeEventArgs(int w, int h) : width(w), height(h) {}
		ResizeEventArgs(ResizeEventArgs&& other) noexcept
			: width(std::move(other.width)), height(std::move(other.height)) {}
        ~ResizeEventArgs() {}
        
        int width;
        int height;
    };
    
    class EventFun
    {
    public:
        EventFun(int id, std::function<void(const EventArgs&)> f) : id(id), func(f) {}
        ~EventFun() {}
        
        int GetId() const { return id; }
        std::function<void(const EventArgs&)> GetFunction() const { return func; }
        
    private:
        int id;
        std::function<void(const EventArgs&)> func;
    };
    
    using Event = std::pair<EventType, EventFun>;
    using EventCallbacks = std::vector<EventFun>;
    
    class EventManager : public Uncopyable
    {
        
    public:
        EventManager();
        ~EventManager();
        
        static EventManager& GetInstance();
        
        void Subscribe(Event&& ev);
        void Unsubscribe(Event&& ev);
        void Dispatch(EventType type, const EventArgs& args);
		void QueueEvent(EventType type, EventArgs* args);
		void PollEvents();
        
    private:
        static EventManager* instance;
        std::map<EventType, EventCallbacks> dispatcher;
		std::queue<std::pair<EventType, EventArgs*>> pendingEvents;
    };
}
/* EventManager_hpp */
