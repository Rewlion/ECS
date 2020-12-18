#pragma once

#include "Entity.h"

#include <assert.h>

Entity::Entity(ComponentManager* pComponentManager, EventManager* pEventManager)
{
    assert(pComponentManager != nullptr);
    assert(pEventManager != nullptr);
    IsActive = true;
    this->pComponentManager = pComponentManager;
    this->pEventManager = pEventManager;
}

void Entity::Activate()
{
    IsActive = true;
}

void Entity::Disable()
{
    RemoveComponents();
    IsActive = false;
}

void Entity::RemoveComponent(const std::string& name)
{
    BaseComponent* component = Components[name];
    pComponentManager->ReleaseComponent(component);

    Components.erase(name);

    pEventManager->SendEvent<ComponentRemovedEvent>(this, component->TypeId);
}

void Entity::RemoveComponents()
{
    for (const auto& pair : Components)
    {
        pComponentManager->ReleaseComponent(pair.second);
    }

    Components.clear();
}

bool Entity::HasComponents(const std::vector<ComponentID>& ids) const
{
    for (const ComponentID& id : ids)
    {
        bool hasComponent = false;
        for (const auto& pair : Components)
        {
            if (pair.second->TypeId == id)
            {
                hasComponent = true;
                break;
            }

        }

        if (hasComponent == false)
            return false;
    }

    return true;
}