#pragma once

#include "ECSEvents.h"
#include "Events/EventManagement.h"
#include "BaseComponent.h"
#include "ComponentManager.h"

#include <assert.h>
#include <map>
#include <string>

typedef size_t EntityID;

class Entity
{
public:
    Entity(ComponentManager* pComponentManager, EventManager* pEventManager);

    void Activate();
    void Disable();

    inline EntityID GetId() const;

    template<class ComponentType>
    ComponentType* AddComponent(const std::string& name);

    void RemoveComponent(const std::string& name);

    template<class ComponentType>
    bool           HasComponent() const;

    template<class ...ComponentTypes>
    bool           HasComponents() const;

    bool           HasComponents(const std::vector<ComponentID>& ids) const;

    template<class ComponentType>
    std::vector<ComponentType*> GetComponents();

    template<class ComponentType>
    ComponentType* GetFirstComponent();

    void RemoveComponents();

private:
    bool IsActive;

    friend class EntityManager;
    EntityID Id;

    std::map<std::string, BaseComponent*> Components;
    EventManager*                         pEventManager;
    ComponentManager*                     pComponentManager;
};

EntityID Entity::GetId() const
{
    return Id;
}

template<class ComponentType>
ComponentType* Entity::AddComponent(const std::string& name)
{
    const bool IsComponentWithNameAlreadyRegisterred = Components.find(name) != Components.end();
    assert(IsComponentWithNameAlreadyRegisterred == false);

    ComponentType* newComponent = pComponentManager->NewComponent<ComponentType>();
    newComponent->Name = name;
    assert(newComponent != nullptr);
    Components.emplace(name, newComponent);

    pEventManager->SendEvent<ComponentAddedEvent>(this, newComponent->TypeId);

    return newComponent;
}

template<class ComponentType>
bool Entity::HasComponent() const
{
    const ComponentTypeId id = ComponentIdFromType::Get<ComponentType>();

    for (const auto& pair : Components)
    {
        if (pair.second->TypeId == id)
            return true;
    }

    return false;
}

template<class ...ComponentTypes>
bool Entity::HasComponents() const
{
    const std::vector<ComponentID> componentIDs = ComponentIdsFromPack::Get<ComponentTypes...>();
    return HasComponents(componentIDs);
}

template<class ComponentType>
std::vector<ComponentType*> Entity::GetComponents()
{
    const ComponentTypeId id = ComponentIdFromType::Get<ComponentType>();
    std::vector<ComponentType*> components;

    for (const auto& pair : Components)
    {
        if (pair.second->TypeId == id)
            components.push_back(reinterpret_cast<ComponentType*>(pair.second));
    }

    return components;
}

template<class ComponentType>
ComponentType* Entity::GetFirstComponent()
{
    const ComponentTypeId id = ComponentIdFromType::Get<ComponentType>();

    for (const auto& pair : Components)
    {
        if (pair.second->TypeId == id)
            return reinterpret_cast<ComponentType*>(pair.second);
    }

    return nullptr;
}
