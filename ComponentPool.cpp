#include "ComponentPool.h"
#include <assert.h>

BaseComponent* ComponentPool::GetComponent(ComponentID id) const
{
    assert(Components.size() > id);
    return Components[id];
}