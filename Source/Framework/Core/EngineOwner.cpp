//
// EngineOwner.hpp
//
// @author Roberto Cano
//

#pragma once

#include "EngineOwner.hpp"

using namespace Framework;
using namespace Framework::Core;
using namespace Framework::Types;

void EngineOwner::setEngine(Types::Engine::PtrType engine)
{
    _engine = engine;
}

const Framework::Types::Engine::PtrType EngineOwner::getEngine() const
{
    assert(_engine);
    return _engine;
}

Framework::Types::Engine::PtrType EngineOwner::getEngine()
{
    assert(_engine);
    return _engine;
}