#pragma once

#include "BaseComponent.h"
#include "ComponentPool.h"

#include <assert.h>
#include <map>

class EventManager;

class ComponentManager
{
public:

    template<class ComponentType>
    ComponentType* NewComponent();

    void ReleaseComponent(const BaseComponent* component);

private:
    template<class ComponentType>
    inline bool DoesPoolExist();

    template<class ComponentType>
    inline void InitializeComponentPool();

private:
    const int PoolInitialCapacity = 10;
    std::map<ComponentTypeId, ComponentPool> Pools;
};

template<class ComponentType>
ComponentType* ComponentManager::NewComponent()
{
    if (DoesPoolExist<ComponentType>() == false)
        InitializeComponentPool<ComponentType>();

    const ComponentTypeId id = ComponentIdFromType::Get<ComponentType>();
    ComponentPool& pool = Pools[id];

    return reinterpret_cast<ComponentType*>(pool.NewComponent<ComponentType>());
}

template<class ComponentType>
bool ComponentManager::DoesPoolExist()
{
    const ComponentTypeId id = ComponentIdFromType::Get<ComponentType>();
    return Pools.find(id) != Pools.end();
}

template<class ComponentType>
inline void ComponentManager::InitializeComponentPool()
{
    const ComponentTypeId id = ComponentIdFromType::Get<ComponentType>();
    ComponentPool newPool;
    newPool.Initialize(PoolInitialCapacity);
    Pools.emplace(id, std::move(newPool));
}