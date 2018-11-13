#pragma once

#include "BaseEvent.h"

template<class T>
class EventPriorityComparator
{
public:
    bool operator()(const T* lv, const T* rv)
    {
        return lv->GetEventObservingPriority() < rv->GetEventObservingPriority();
    }
};

class EventIdFromType
{
public:
    template<class EventType>
    static EventTypeId Get()
    {
        static_assert((std::is_base_of<BaseEvent, EventType>::value == true) && (std::is_same<BaseEvent, EventType>::value == false),
            "Unable to get an event type id from the Class that is not derived from IEvent.");

        static EventTypeId typeID = typeCount++;

        return typeID;
    }
private:
    static EventTypeId typeCount;
};