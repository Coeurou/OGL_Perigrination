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
        ~EventArgs() {}
    };
    
    class MouseEventArgs : public EventArgs
    {
    public:
        MouseEventArgs(int b, int s, double pX, double pY)
            : button(b), state(s), posX(pX), posY(pY) {}
        ~MouseEventArgs() {}
        
        int button;
        int state;
        double posX;
        double posY;
    };
    
    class KeyEventArgs : public EventArgs
    {
    public:
        KeyEventArgs(int k, int w, int h) : key(k), width(w), height(h) {}
        ~KeyEventArgs() {}
    
        int key;
        int width;
        int height;
    };
    
    class ResizeEventArgs : public EventArgs
    {
    public:
        ResizeEventArgs(int w, int h) : width(w), height(h) {}
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
        
        void Subscribe(Event ev);
        void Unsubscribe(Event ev);
        void Dispatch(EventType type, const EventArgs& args);
        
    private:
        static EventManager* instance;
        std::map<EventType, EventCallbacks> dispatcher;
    };
}
/* EventManager_hpp */
