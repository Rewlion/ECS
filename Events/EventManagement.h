#pragma once

#include "EventUtils.h"
#include "BaseEvent.h"

#include <assert.h>
#include <algorithm>
#include <functional>
#include <map>
#include <memory>
#include <stack>
#include <queue>
#include <vector>

typedef unsigned int ObserverID;
class EventManager;

enum EventObservingPriority
{
    Priority_Low = 0,
    Priority_Critical = 1000
};

class EventObserver
{
public:
    friend EventManager;

public:
    explicit inline EventObserver(EventManager* peventManager, EventObservingPriority priority);

    template<class EventType>
    void OnEvent(std::function<void(BaseEvent*)> slot);

    inline EventObservingPriority GetEventObservingPriority() const;

private:
    template<class EventType>
    void Observe(BaseEvent* pEvent);
    //todo: unregister event listening

private:
    EventObservingPriority Priority;
    EventManager* pEventManager;
    ObserverID ID;
    std::map<EventTypeId, std::function<void(BaseEvent*)>> Slots;
};

/////////
class EventManager final
{
public:

    template<class EventType>
    void RegisterEvent();

    template<class EventType>
    void ConnectObserver(EventObserver* observer);

    template<class EventType, class ...Ts>
    void SendEvent(Ts... args);

private:
    //typedef std::priority_queue<EventObserver*, std::vector<EventObserver*>, EventPriorityComparator<EventObserver*>> ObserverQueue;
    std::map<EventTypeId, std::vector<EventObserver*>> RegisteredEvents;
    //std::map<EventTypeId, std::stack<ObserverID>> ReusableObserverIDsMap;
};
/////////////////////////////////////////////
inline EventObserver::EventObserver(EventManager* peventManager, EventObservingPriority priority)
    : pEventManager(peventManager)
    , Priority(priority)
{
}

template<class EventType>
void EventObserver::OnEvent(std::function<void(BaseEvent*)> slot)
{
    const EventTypeId eventID = EventIdFromType::Get<EventType>();
    Slots[eventID] = slot;
    pEventManager->ConnectObserver<EventType>(this);
}

EventObservingPriority EventObserver::GetEventObservingPriority() const
{
    return Priority;
}

template<class EventType>
void EventObserver::Observe(BaseEvent* pEvent)
{
    const EventTypeId eventID = EventIdFromType::Get<EventType>();
    assert((Slots.find(eventID) != Slots.end()) && "Observer connected to the event but doesn't have any slots to observe.");

    const auto& slot = Slots[eventID];
    slot(pEvent);
}
/////////////////////////////////////////////
template<class EventType>
void EventManager::RegisterEvent() //deprecated
{
    const EventTypeId id = EventIdFromType::Get<EventType>();
    assert((RegisteredEvents.find(id) == RegisteredEvents.end()) && "Unable to register a new event type: event already registered.");
    RegisteredEvents[id] = std::vector<EventObserver*>();
    //ReusableObserverIDsMap[id] = std::stack<ObserverID>();
}

template<class EventType>
void EventManager::ConnectObserver(EventObserver* observer)
{
    const EventTypeId eventID = EventIdFromType::Get<EventType>();
    if(RegisteredEvents.find(eventID) == RegisteredEvents.end())
        RegisteredEvents[eventID] = std::vector<EventObserver*>();

    const ObserverID id = static_cast<ObserverID>(RegisteredEvents[eventID].size());
    RegisteredEvents[eventID].push_back(observer);
    std::sort(RegisteredEvents[eventID].begin(), RegisteredEvents[eventID].end(), EventPriorityComparator<EventObserver>());

    observer->ID = id;
}

template<class EventType, class ...Ts>
void EventManager::SendEvent(Ts... args)
{
    const EventTypeId eventID = EventIdFromType::Get<EventType>();

    if(RegisteredEvents.find(eventID) != RegisteredEvents.end())
    { 
      EventType* e = new EventType(args...);

        const auto& observers = RegisteredEvents[eventID];
        for (auto& observer : observers)
            observer->Observe<EventType>(e);

        delete e;
    }
}