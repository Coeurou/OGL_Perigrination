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
    std::unique_ptr<EventManager> EventManager::instance = nullptr;
	bool EventManager::instanceDeleted = false;

    EventManager::EventManager()
    {}
    
    EventManager::~EventManager()
    {
		EventManager::instanceDeleted = true;
	}
    
    EventManager* EventManager::GetInstance()
    {
        if (instance == nullptr) {
            instance = std::make_unique<EventManager>();
        }
        return instance.get();
    }
    
    void EventManager::Subscribe(EventType type, EventListener* listener)
    {
        dispatcher[type].push_back(listener);
    }
    
    void EventManager::Unsubscribe(EventType type, EventListener* listener)
    {   
        dispatcher[type].erase(std::remove_if(dispatcher[type].begin(), dispatcher[type].end(), [listener](EventListener* l) { return l == listener; }));
    }
    
    void EventManager::Dispatch(Event e)
    {
		for (auto& listener : dispatcher[e.GetEventType()]) {
			listener->OnEvent(e);
		}
    }

	void EventManager::QueueEvent(Event e)
	{
		pendingEvents.push(e);
	}

	void EventManager::PollEvents()
	{
		while (!pendingEvents.empty()) {
			Dispatch(pendingEvents.front());
			pendingEvents.pop();
		}
	}
}