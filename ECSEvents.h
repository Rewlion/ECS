#pragma once

#include "Events/BaseEvent.h"

class Entity;
typedef size_t EntityID;
typedef unsigned int ComponentTypeId;

struct EntityAddedEvent : public BaseEvent
{
    typedef std::shared_ptr<EntityAddedEvent> Ptr;

    EntityAddedEvent(Entity* entity)
        : AddedEntity(entity)
    {
    }

    Entity* AddedEntity;
};

struct EntityReleasedEvent : public BaseEvent
{
    typedef std::shared_ptr<EntityReleasedEvent> Ptr;

    EntityReleasedEvent(Entity* entity)
        : RemovedEntity(entity)
    {
    }
    Entity* RemovedEntity;
};

struct ComponentRemovedEvent : public BaseEvent
{
    typedef std::shared_ptr<ComponentRemovedEvent> Ptr;

    ComponentRemovedEvent(Entity* owner, ComponentTypeId removedComponentID)
        : Owner(owner)
        , RemovedComponentID(removedComponentID)
    {
    }
    Entity* Owner;
    ComponentTypeId RemovedComponentID;
};

struct ComponentAddedEvent : public BaseEvent
{
    typedef std::shared_ptr<ComponentAddedEvent> Ptr;

    ComponentAddedEvent(Entity* owner, ComponentTypeId addedComponentID)
        : Owner(owner)
        , AddedComponentID(addedComponentID)
    {
    }
    Entity* Owner;
    ComponentTypeId AddedComponentID;
};