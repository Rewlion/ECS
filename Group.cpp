#include "Group.h"
#include "Context.h"
#include "ECSEvents.h"
#include "Entity.h"

Group::Group(Context* context, const std::vector<ComponentID>& requirableComponentIDs)
    : CurrentContext(context)
    , RequirableComponentIDs(requirableComponentIDs)
    , Observer(CurrentContext->GetEventManager(), Priority_Critical)
{
    Observer.OnEvent<EntityAddedEvent>([this](BaseEvent::Ptr event)
    {
        OnEntityAdded(event);
    });

    Observer.OnEvent<EntityReleasedEvent>([this](BaseEvent::Ptr event)
    {
        OnEntityReleased(event);
    });

    Observer.OnEvent<ComponentAddedEvent>([this](BaseEvent::Ptr event)
    {
        OnComponentAdded(event);
    });

    Observer.OnEvent<ComponentRemovedEvent>([this](BaseEvent::Ptr event)
    {
        OnComponentRemoved(event);
    });

    const auto entities = context->GetEntityManager()->GetEntities();
    for (const auto& entity : entities)
    {
        if (entity->HasComponents(RequirableComponentIDs))
            Entities.push_back(entity);
    }
}

void Group::OnEntityAdded(BaseEvent::Ptr event)
{
    EntityAddedEvent::Ptr castedEvent = std::dynamic_pointer_cast<EntityAddedEvent>(event);
    assert(castedEvent != nullptr);
    Entity* addedEntity = castedEvent->AddedEntity;

    if (addedEntity->HasComponents(RequirableComponentIDs))
    {
        if (ReusableEntityIds.empty() == false)
        {
            const PositionInCache position = ReusableEntityIds.top();
            assert((Entities[position] == nullptr) && "Group cached ids has an activated entity.");
            Entities[position] = addedEntity;

            return;
        }
        else
        {
            Entities.push_back(addedEntity);
        }
    }
}

void Group::OnEntityReleased(BaseEvent::Ptr event)
{
    EntityReleasedEvent::Ptr castedEvent = std::dynamic_pointer_cast<EntityReleasedEvent>(event);
    assert(castedEvent != nullptr);
    const Entity* removedEntity = castedEvent->RemovedEntity;

    if (removedEntity->HasComponents(RequirableComponentIDs))
    {
        auto[positionInCache, isFound] = FindPositionWithID(removedEntity->GetId());
        assert((isFound) && "Entity sutisfies with required component ids but it hasn't been in group when EntityReleased trigerred.");
        assert((Entities[positionInCache] == nullptr) && "chunga changa.");

        Entities[positionInCache] = nullptr;
        ReusableEntityIds.push(positionInCache);
    }

    
}

void Group::OnComponentAdded(BaseEvent::Ptr event)
{
    ComponentAddedEvent::Ptr castedEvent = std::dynamic_pointer_cast<ComponentAddedEvent>(event);
    Entity* owner = castedEvent->Owner; // ownerid -> owner(entity)

    auto[positionInCache, isFound] = FindPositionWithID(owner->GetId());
    if (isFound == false)
    {
        if (owner->HasComponents(RequirableComponentIDs))
        {
            if (ReusableEntityIds.empty() == false)
            {
                const PositionInCache position = ReusableEntityIds.top();
                assert((Entities[position] == nullptr) && "Removed entity in group is not nullptr.");
                Entities[position] = owner;
                ReusableEntityIds.pop();

                return;
            }
            else
            {
                Entities.push_back(owner);
            }
        }
    }
}

void Group::OnComponentRemoved(BaseEvent::Ptr event)
{
    ComponentRemovedEvent::Ptr castedEvent = std::dynamic_pointer_cast<ComponentRemovedEvent>(event);
    Entity* owner = castedEvent->Owner;

    auto[positionInCache, isFound] = FindPositionWithID(owner->GetId());
    if (isFound)
    {
        Entity* entity = Entities[positionInCache];
        if (entity->HasComponents(RequirableComponentIDs) == false)
            Entities[positionInCache] = nullptr;

        ReusableEntityIds.push(positionInCache);
    }
}

std::tuple<PositionInCache, bool> Group::FindPositionWithID(const EntityID id) const
{
    for (unsigned int i = 0; i < Entities.size(); ++i)
    {
        if (Entities[i] == nullptr)
            continue;

        if (id == Entities[i]->GetId())
            return { i, true };
    }

    return { -1, false };
}