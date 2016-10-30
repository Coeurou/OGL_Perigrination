//
//  EventManager.hpp
//  OGLPlatform
//
//  Created by Guillaume Trollé on 14/08/2016.
//  Copyright © 2016 Guillaume Trollé. All rights reserved.
//

#pragma once

#include "Uncopyable.hpp"
#include "EventListener.hpp"
#include <map>
#include <memory>
#include <mutex>
#include <vector>
#include <queue>

namespace gs
{ 
    
    class EventManager : public Uncopyable
    {
        
    public:
        EventManager();
        ~EventManager();
        
        static EventManager* GetInstance();
        
        void Subscribe(EventType type, IEventListener* listener);
        void Unsubscribe(EventType type, IEventListener* listener);
        void Dispatch(Event e);
		void QueueEvent(Event e);
		void PollEvents();
		static bool IsInstanceDeleted() { return EventManager::instanceDeleted; }
        
    private:
		static std::unique_ptr<EventManager> instance;
		static bool instanceDeleted;
        std::map<EventType, std::vector<IEventListener*>> dispatcher;
		std::queue<Event> pendingEvents;
		std::queue<std::pair<EventType, IEventListener*>> pendingSubscribers;
		std::queue<std::pair<EventType, IEventListener*>> pendingUnsubscribers;

		void PollSubscribers();
	};
}
/* EventManager_hpp */
