#pragma once

#include "BaseComponent.h"
#include "ComponentUtils.h"

#include <vector>
#include <stack>

class ComponentPool
{
public:
    inline void    Initialize(const unsigned int InitialCapacity);
    template<class ComponentType>
    BaseComponent* NewComponent();
    inline void    ReleaseComponent(ComponentID id);
    BaseComponent* GetComponent(ComponentID id) const;

private:
    typedef unsigned int PositionInPool;
    std::stack<PositionInPool>  ReusablePoolPositions;
    std::vector<BaseComponent*> Components;
};

template<class ComponentType>
BaseComponent* ComponentPool::NewComponent()
{

    const ComponentTypeId typeId = ComponentIdFromType::Get<ComponentType>();
    if (ReusablePoolPositions.empty() == false)
    {
        const ComponentID position = ReusablePoolPositions.top();
        ReusablePoolPositions.pop();

        return Components[position];
    }
    else
    {
        const ComponentID id = Components.size();
        BaseComponent* newComponent = new ComponentType();
        newComponent->TypeId = typeId;
        newComponent->Id = id;

        Components.push_back(newComponent);

        return newComponent;
    }
}

void ComponentPool::Initialize(const unsigned int InitialCapacity)
{
    Components.reserve(InitialCapacity);
}

void ComponentPool::ReleaseComponent(ComponentID id)
{
    ReusablePoolPositions.push(id);
}