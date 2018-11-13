#pragma once

class Context;

typedef unsigned int SystemTypeId;

class BaseSystem
{
public:
    inline explicit BaseSystem(Context* pcontext)
        : pContext(pcontext)
    {
    }

    virtual ~BaseSystem() {}

protected:
    Context* pContext;
};

class InitializationSystem : public BaseSystem
{
public:
    inline explicit InitializationSystem(Context* pcontext)
        : BaseSystem(pcontext)
    {
    }

    virtual ~InitializationSystem() {}

    virtual void Initialize() = 0;

};

class LogicSystem : public BaseSystem
{
public:
    inline explicit LogicSystem(Context* pcontext)
        : BaseSystem(pcontext)
    {
    }

    virtual ~LogicSystem() {}

    virtual void Update(const double dt) = 0;
};

class CleanupSystem : public BaseSystem
{
public:
    inline explicit CleanupSystem(Context* pcontext)
        : BaseSystem(pcontext)
    {
    }

    virtual ~CleanupSystem() {}

    virtual void Clean() = 0;
};