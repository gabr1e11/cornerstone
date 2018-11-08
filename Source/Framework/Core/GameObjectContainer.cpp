//
// GameObjectContainer.cpp
//
// @author Roberto Cano
//

#pragma once

#include "GameObjectContainer.hpp"

#include "GameObject.hpp"

using namespace Framework;
using namespace Framework::Core;
using namespace Framework::Types;

void GameObjectContainer::addGameObject(Types::GameObject::PtrType gameObject)
{
	_notStartedObjects.push(gameObject);
	doAddGameObject(std::move(gameObject));
}

bool GameObjectContainer::hasGameObject(const Core::GameObject& gameObject) const
{
	return hasGameObject(gameObject.getInstanceId());
}

bool GameObjectContainer::hasGameObject(const InstanceId& instanceId) const
{
	auto goMapIter = _gameObjects.find(instanceId);
	return goMapIter != _gameObjects.end();
}

void GameObjectContainer::removeGameObject(const Core::GameObject& gameObject)
{
	const InstanceId& instanceId = gameObject.getInstanceId();

	removeGameObject(instanceId);
}

void GameObjectContainer::removeGameObject(const InstanceId& instanceId)
{
	_gameObjects.erase(instanceId);
}

void GameObjectContainer::internalInit()
{
	for (auto& gameObject : _gameObjects)
	{
		gameObject.second->internalInit();
	}
}

void GameObjectContainer::internalStart()
{
	updateNotStarted();

	for (auto& gameObject : _gameObjects)
	{
		gameObject.second->internalStart();
	}
}

void GameObjectContainer::internalUpdate(float dt)
{
	updateNotStarted();

	for (auto& gameObject : _gameObjects)
	{
		gameObject.second->internalUpdate(dt);
	}
}

void GameObjectContainer::updateNotStarted()
{
	if (_notStartedObjects.empty())
	{
		return;
	}

	while (!_notStartedObjects.empty())
	{
		Types::GameObject::WeakPtrType gameObjectWPtr = _notStartedObjects.front();
		_notStartedObjects.pop();

		if (auto gameObject = gameObjectWPtr.lock())
		{
			gameObject->internalStart();
		}
	}
}

void GameObjectContainer::doAddGameObject(Types::GameObject::PtrType gameObject)
{
	const InstanceId& instanceId = gameObject->getInstanceId();
	_gameObjects.insert(std::pair<InstanceId, Types::GameObject::PtrType>(instanceId, std::move(gameObject)));
}