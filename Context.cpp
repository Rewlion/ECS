#include "Context.h"
#include "ECSEvents.h"

Context::Context()
    : EntManager(&CompManager, &EvntManager)
    , UserData(nullptr)
{
    RegisterDefaultECSEvents();
}

void Context::RunInitializationSystems()
{
    for (const auto& system : InitializationSystems)
        system->Initialize();
}

void Context::UpdateSystems(const float dt)
{
    for (const auto& system : LogicSystems)
        system->Update(dt);

    for (const auto& system : CleanupSystems)
        system->Clean();
}

void Context::RegisterDefaultECSEvents()
{
    EvntManager.RegisterEvent<EntityAddedEvent>();
    EvntManager.RegisterEvent<EntityReleasedEvent>();
    EvntManager.RegisterEvent<ComponentAddedEvent>();
    EvntManager.RegisterEvent<ComponentRemovedEvent>();

}