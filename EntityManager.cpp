#include "EntityManager.h"
#include "ECSEvents.h"
#include "Events/EventManagement.h"

EntityManager::EntityManager(ComponentManager* pComponentManager, EventManager* pEventManager)
{
    assert(pComponentManager != nullptr);
    assert(pEventManager != nullptr);

    this->pEventManager = pEventManager;
    this->pComponentManager = pComponentManager;
}

Entity* EntityManager::NewEntity()
{
    Entity* newEntity = nullptr;
    if (ReusablePositionsInPool.empty())
    {
        const EntityID id = Entities.size();
        newEntity = new Entity(pComponentManager, pEventManager);
        newEntity->Id = id;
        
        Entities.push_back(newEntity);
    }
    else
    {
        const EntityID id = ReusablePositionsInPool.top();
        ReusablePositionsInPool.pop();
        newEntity = Entities[id];
    }

    pEventManager->SendEvent<EntityAddedEvent>(newEntity);

    return newEntity;
}

void EntityManager::ReleaseEntity(Entity* entity)
{
    entity->Disable();
    ReusablePositionsInPool.push(entity->Id);

    pEventManager->SendEvent<EntityReleasedEvent>(entity);
}