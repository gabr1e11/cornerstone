//
// CodeExecution.hpp
//
// @author Roberto Cano
//

#pragma once

#include "CodeExecution.hpp"

using namespace Framework::Components;
using namespace Framework::Core;

CodeExecution::CodeExecution(UpdateFunction& updateFunction)
    : _updateFunction(updateFunction)
{
    // Empty on purpose
}

void CodeExecution::init()
{
    // Empty on purpose
}

void CodeExecution::start()
{
    // Empty on purpose
}

void CodeExecution::update(float dt)
{
    if (_updateFunction)
    {
        _updateFunction(dt);
    }
}