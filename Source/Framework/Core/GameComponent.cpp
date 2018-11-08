//
// GameObjectComponent.cpp
//
// @author Roberto Cano
//

#include "GameComponent.hpp"

#include "Framework/Utils/Utils.hpp"

using namespace Framework::Core;
using namespace Framework::Utils;

GameComponent::GameComponent()
{
	// Empty on purpose
}

const std::string& GameComponent::getComponentId() const
{
	return _componentId;
}

void GameComponent::internalInit()
{
	_componentId = GetComponentIdFromThis(*this);
	generateInstanceId(_componentId);

	init();
}

void GameComponent::internalStart()
{
	start();
}

void GameComponent::internalUpdate(float dt)
{
	update(dt);
}

void GameComponent::init()
{
	// Empty on purpose
}

void GameComponent::start()
{
	// Empty on purpose
}

void GameComponent::update(float /*dt*/)
{
	// Empty on purpose
}