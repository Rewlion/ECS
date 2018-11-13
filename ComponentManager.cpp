#include "ComponentManager.h"

void ComponentManager::ReleaseComponent(const BaseComponent* component)
{
    assert(component != nullptr);

    const ComponentTypeId poolId = component->TypeId;
    ComponentPool& pool = Pools[poolId];
    pool.ReleaseComponent(component->Id);
}