//
// EngineOwner.hpp
//
// @author Roberto Cano
//

#pragma once

#include "GameObjectOwner.hpp"

#include "GameObject.hpp"

using namespace Framework;
using namespace Framework::Core;

bool GameObjectOwner::hasOwner() const
{
	return _gameObject.lock() != nullptr;
}

void GameObjectOwner::_setOwner(Types::GameObject::WeakPtrType gameObject)
{
	_gameObject = std::move(gameObject);
}

const Core::GameObject& GameObjectOwner::getOwner() const
{
	auto gameObjectPtr = _gameObject.lock();
	assert(gameObjectPtr);
	return *gameObjectPtr;
}

Core::GameObject& GameObjectOwner::getOwner()
{
	auto gameObjectPtr = _gameObject.lock();
	assert(gameObjectPtr);
	return *gameObjectPtr;
}