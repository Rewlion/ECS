#pragma once
#include "Events/EventManagement.h"

#include <vector>
#include <stack>

typedef unsigned int ComponentID;
typedef unsigned int PositionInCache;
typedef size_t EntityID;

class Entity;
class Context;

class Group
{
public:
    Group(Context* context,  const std::vector<ComponentID>& requirableComponentIDs);

    inline const std::vector<Entity*>&    GetEntities() const;
    inline Entity*                        GetFirstNotNullEntity() const;
    inline const std::vector<ComponentID> GetRequirableComponentIDs() const;

private:
    void                                  OnEntityAdded(BaseEvent* event);
    void                                  OnEntityReleased(BaseEvent* event);
    void                                  OnComponentAdded(BaseEvent* event);
    void                                  OnComponentRemoved(BaseEvent* event);

    std::tuple<PositionInCache, bool>     FindPositionWithID(const EntityID id) const;

private:
    Context* CurrentContext;

    std::vector<Entity*> Entities;
    std::stack<PositionInCache> ReusableEntityIds;

    std::vector<ComponentID> RequirableComponentIDs;

    EventObserver Observer;
};

inline const std::vector<Entity*>& Group::GetEntities() const
{
    return Entities;
}

Entity* Group::GetFirstNotNullEntity() const
{
    for (Entity* e : Entities)
        if (e != nullptr)
            return e;

    return nullptr;
}

inline const std::vector<ComponentID> Group::GetRequirableComponentIDs() const
{
    return RequirableComponentIDs;
}