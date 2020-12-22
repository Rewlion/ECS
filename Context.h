#pragma once

#include "Events/EventManagement.h"
#include "EntityManager.h"
#include "ComponentManager.h"
#include "BaseSystems.h"
#include "Group.h"

#include <iterator>
#include <vector>

class Context final
{
public:
    Context();
    
    void RunInitializationSystems();
    void UpdateSystems(const float dt);

    template<class... Components>
    Group* GetGroup();

    inline void AddInitializationSystems(std::initializer_list<InitializationSystem*> systems);
    inline void AddLogicSystems(std::initializer_list<LogicSystem*> systems);
    inline void AddCleanupSystems(std::initializer_list<CleanupSystem*> systems);

    inline EventManager* GetEventManager();
    inline EntityManager* GetEntityManager();

    inline void SetUserData(void* data);
    inline void* GetUserData();

    inline bool CheckIfShouldStopSimulation() const;

private:
    void RegisterDefaultECSEvents();

private:
    EventManager                       EvntManager;
    ComponentManager                   CompManager;
    EntityManager                      EntManager;
    std::vector<InitializationSystem*> InitializationSystems;
    std::vector<LogicSystem*>          LogicSystems;
    std::vector<CleanupSystem*>        CleanupSystems;
    std::vector<Group*>                Groups;
    void*                              UserData;

    bool ShouldStopSimulation;
};

template<class... ComponentsToObserve>
Group* Context::GetGroup()
{
    const std::set<ComponentID> idsToObserve = ComponentIdsFromPack::GetSet<ComponentsToObserve...>();
    for (const auto& group : Groups)
    {
        std::vector<ComponentID> requiredByGroupIDs = group->GetRequirableComponentIDs();
        std::set<ComponentID> groupIdsSet(requiredByGroupIDs.begin(), requiredByGroupIDs.end());

        if (idsToObserve == groupIdsSet)
            return group;
    }

    Group* newGroup{ new Group{ this, ComponentIdsFromPack::Get<ComponentsToObserve...>() } };
    Groups.push_back(newGroup);

    return newGroup;
}

void Context::AddInitializationSystems(std::initializer_list<InitializationSystem*> systems)
{
    InitializationSystems.insert(InitializationSystems.end(), std::make_move_iterator(systems.begin()), std::make_move_iterator(systems.end()));
}

void Context::AddLogicSystems(std::initializer_list<LogicSystem*> systems)
{
    LogicSystems.insert(LogicSystems.end(), std::make_move_iterator(systems.begin()), std::make_move_iterator(systems.end()));
}

void Context::AddCleanupSystems(std::initializer_list<CleanupSystem*> systems)
{
    CleanupSystems.insert(CleanupSystems.end(), std::make_move_iterator(systems.begin()), std::make_move_iterator(systems.end()));
}

EventManager* Context::GetEventManager()
{
    return &EvntManager;
}

EntityManager* Context::GetEntityManager()
{
    return &EntManager;
}

void Context::SetUserData(void* data)
{
  UserData = data;
}

void* Context::GetUserData()
{
  return UserData;
}

bool Context::CheckIfShouldStopSimulation() const
{
    return ShouldStopSimulation;
}