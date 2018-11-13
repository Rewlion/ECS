#pragma once

#include "Entity.h"

#include <vector>
#include <stack>

class Entity;
class ComponentManager;
class EventManager;

class EntityManager
{
public:
    EntityManager(ComponentManager* pComponentManager, EventManager* pEventManager);

    Entity* NewEntity();
    void    ReleaseEntity(Entity* entity);
    
    inline const std::vector<Entity*>& GetEntities();
private:
    typedef unsigned int PositionInPool;

    EventManager*              pEventManager;
    ComponentManager*          pComponentManager;
    std::vector<Entity*>       Entities;
    std::stack<PositionInPool> ReusablePositionsInPool;
};

const std::vector<Entity*>& EntityManager::GetEntities()
{
    return Entities;
}