//
//  EventManager.cpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 14/08/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#include "EventManager.hpp"
#include <algorithm>

namespace gs
{
    EventManager* EventManager::instance = nullptr;
    
    EventManager::EventManager()
    {}
    
    EventManager::~EventManager()
    {}
    
    EventManager& EventManager::GetInstance()
    {
        if (instance == nullptr) {
            instance = new EventManager();
        }
        return *instance;
    }
    
    void EventManager::Subscribe(Event ev)
    {
        dispatcher[ev.first].push_back(ev.second);
    }
    
    void EventManager::Unsubscribe(Event ev)
    {   
        dispatcher[ev.first].erase(std::remove_if(dispatcher[ev.first].begin(), dispatcher[ev.first].end(), [&](EventFun e) { return ev.second.GetId() == e.GetId(); }));
    }
    
    void EventManager::Dispatch(EventType type, const EventArgs& args)
    {
        for (auto callback : dispatcher[type]) {
            callback.GetFunction()(args);
        }
    }
}