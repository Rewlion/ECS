#pragma once

#include "BaseComponent.h"

#include <vector>
#include <set>

template<class ComponentType>
inline void InsureComponentDerivedFromCorrectBase()
{
    static_assert((std::is_base_of<BaseComponent, ComponentType>::value == true) && (std::is_same<BaseComponent, ComponentType>::value == false),
        "Component class is not derived from BaseComponent.");
}

class ComponentIdFromType
{
public:
    template<class ComponentType>
    constexpr static ComponentID Get()
    {
        InsureComponentDerivedFromCorrectBase<ComponentType>();
        static ComponentID typeID = typeCount++;

        return typeID;
    }

private:
    static ComponentID typeCount;
};

class ComponentIdsFromPack
{
public:
    template<class ...Args>
    constexpr static std::vector<ComponentID> Get()
    {
        return { ComponentIdFromType::Get<Args>()... };
    }

    template<class ...Args>
    constexpr static std::set<ComponentID> GetSet()
    {
        return { ComponentIdFromType::Get<Args>()... };
    }
};