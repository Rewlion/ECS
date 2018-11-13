#pragma once

#include <string>
#include <vector>

typedef unsigned int ComponentID;
typedef unsigned int ComponentTypeId;

struct BaseComponent
{
    std::string              Name;
    ComponentID              Id;
    ComponentTypeId          TypeId;
    std::vector<std::string> Tags;
};