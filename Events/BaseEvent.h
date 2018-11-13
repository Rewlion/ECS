#pragma once

#include <memory>

typedef unsigned int EventTypeId;

struct BaseEvent
{
    typedef std::shared_ptr<BaseEvent> Ptr;

    EventTypeId ID;

    virtual ~BaseEvent() {}
};