//
// GameComponentContainer.cpp
//
// @author Roberto Cano
//

#include "GameComponentContainer.hpp"

using namespace Framework;
using namespace Framework::Core;
using namespace Framework::Types;

void GameComponentContainer::addComponent(Types::GameComponent::PtrType component)
{
	const ComponentId& componentId = component->getComponentId();
	const InstanceId& instanceId = component->getInstanceId();

	_notStartedComponents.push(component);

	// Dependency injection
	Types::GameObject::PtrType gameObjectPtr = shared_from_this();
	component->_setOwner(gameObjectPtr);

	// Map by Instance Id
	auto insertionIter = _componentsMapByInstanceId.insert(std::pair<InstanceId, Types::GameComponent::PtrType>(instanceId, std::move(component)));
	assert(insertionIter.second);

	// Map by Component Id
	auto componentsMapIter = _componentsMapByComponentId.find(componentId);
	if (componentsMapIter != _componentsMapByComponentId.end())
	{
		componentsMapIter->second.emplace(instanceId);
	}
	else
	{
		_componentsMapByComponentId.emplace(componentId, std::set<InstanceId>{instanceId});
	}
}

bool GameComponentContainer::removeComponent(const Core::GameComponent& component)
{
	const InstanceId& instanceId = component.getInstanceId();
	return removeComponent(instanceId);
}

bool GameComponentContainer::removeComponent(const InstanceId& instanceId)
{
	bool retValue = true;

	auto findIter = _componentsMapByInstanceId.find(instanceId);
	if (findIter == _componentsMapByInstanceId.end())
	{
		return false;
	}

	const GameComponent& component = *(findIter->second);
	const ComponentId& componentId = component.getComponentId();

	// Map by Component Id
	auto componentsMapIter = _componentsMapByComponentId.find(componentId);
	if (componentsMapIter != _componentsMapByComponentId.end())
	{
		componentsMapIter->second.erase(instanceId);
	}
	else
	{
		retValue = false;
	}

	// Map by Instance Id
	_componentsMapByInstanceId.erase(findIter);

	return retValue;
}

bool GameComponentContainer::hasComponent(const Core::GameComponent& component) const
{
	const InstanceId& instanceId = component.getInstanceId();
	return hasComponent(instanceId);
}

bool GameComponentContainer::hasComponents(const ComponentId& componentId) const
{
	auto componentsMapIter = _componentsMapByComponentId.find(componentId);
	return componentsMapIter != _componentsMapByComponentId.end();
}

bool GameComponentContainer::hasComponent(const InstanceId& instanceId) const
{
	auto instancesMapIter = _componentsMapByInstanceId.find(instanceId);
	return instancesMapIter != _componentsMapByInstanceId.end();
}

const std::set<InstanceId>& GameComponentContainer::getComponentsIds(const ComponentId& componentId) const
{
	auto componentsMapIter = _componentsMapByComponentId.find(componentId);
	assert(componentsMapIter != _componentsMapByComponentId.end());

	return componentsMapIter->second;
}

void GameComponentContainer::internalInit()
{
	for (auto& componentPair : _componentsMapByInstanceId)
	{
		componentPair.second->init();
	}
}

void GameComponentContainer::internalStart()
{
	for (auto& componentPair : _componentsMapByInstanceId)
	{
		componentPair.second->start();
	}
}

void GameComponentContainer::internalUpdate(float dt)
{
	for (auto& componentPair : _componentsMapByInstanceId)
	{
		componentPair.second->update(dt);
	}
}

void GameComponentContainer::updateNotStarted()
{
	if (_notStartedComponents.empty())
	{
		return;
	}

	while (!_notStartedComponents.empty())
	{
		Types::GameComponent::WeakPtrType gameComponentWPtr = _notStartedComponents.front();
		_notStartedComponents.pop();

		if (auto gameComponent = gameComponentWPtr.lock())
		{
			gameComponent->internalStart();
		}
	}
}