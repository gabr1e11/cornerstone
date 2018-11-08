//
// GameObject.cpp
//
// @author Roberto Cano
//

#pragma once

#include "GameObject.hpp"

#include "Engine.hpp"

using namespace Framework;
using namespace Framework::Core;
using namespace Framework::Types;

void Core::GameObject::setWorldPosition(const Point2D& newPosition)
{
	_worldPosition = newPosition;
	updateLocalPosition(_worldPosition);
}

const Point2D& Core::GameObject::getWorldPosition() const
{
	return _worldPosition;
}

void Core::GameObject::setLocalPosition(const Point2D& newPosition)
{
	_localPosition = newPosition;
	updateWorldPosition(_localPosition);
}

const Point2D& Core::GameObject::getLocalPosition() const
{
	return _localPosition;
}

void Core::GameObject::updateWorldPosition(const Point2D& newPosition)
{
	if (!hasOwner())
	{
		_worldPosition = newPosition;
	}
	else
	{
		const GameObject& owner = getOwner();
		_worldPosition = owner.getWorldPosition() + newPosition;
	}
}

void Core::GameObject::updateLocalPosition(const Point2D& newPosition)
{
	if (!hasOwner())
	{
		_localPosition = newPosition;
	}
	else
	{
		const GameObject& owner = getOwner();
		_localPosition = newPosition - owner.getWorldPosition();
	}
}

Point2D Core::GameObject::getAnchorPointLocalPosition() const
{
	const Size2D size = getSize();
	return size * getAnchorPoint();
}

void Core::GameObject::_setOwner(Types::GameObject::WeakPtrType gameObject)
{
	GameObjectOwner::_setOwner(gameObject);

	// Local position takes precedence over world position
	updateWorldPosition(_localPosition);
}

void Core::GameObject::setZIndex(int zIndex)
{
	_zIndex = zIndex;
}

int Core::GameObject::getZIndex() const
{
	return _zIndex;
}

void Core::GameObject::internalInit()
{
	const std::string& classId = typeid(*this).name();
	generateInstanceId(classId);

	GameObjectContainer::internalInit();
	GameComponentContainer::internalInit();

	init();
}

void Core::GameObject::internalStart()
{
	GameObjectContainer::internalStart();
	GameComponentContainer::internalStart();

	start();
}

void Core::GameObject::internalUpdate(float dt)
{
	GameObjectContainer::internalUpdate(dt);
	GameComponentContainer::internalUpdate(dt);

	update(dt);
}

void Core::GameObject::addGameObject(Types::GameObject::PtrType gameObject)
{
	auto ptr = shared_from_this();
	gameObject->setEngine(getEngine());
	gameObject->_setOwner(ptr);

	GameObjectContainer::addGameObject(std::move(gameObject));
}

void Core::GameObject::setSize(const Size2D& size)
{
	_size = size;
}

const Size2D& Core::GameObject::getSize() const
{
	return _size;
}

bool Core::GameObject::isPointInside(const Point2D& point) const
{
	const Point2D& goWorldPosition = getWorldPosition();
	const Point2D& goAnchorLocalPos = getAnchorPointLocalPosition();

	const Point2D& localPointPosition = point - goWorldPosition + goAnchorLocalPos;

	// Bounding box check
	const Size2D size = getSize();
	return (localPointPosition.x >= 0.0f && localPointPosition.x <= size.x &&
		localPointPosition.y >= 0.0f && localPointPosition.y <= size.x);
}

void Core::GameObject::init()
{
	// Empty on purpose, allow basic GameObjects to just work
}

void Core::GameObject::start()
{
	// Empty on purpose, allow basic GameObjects to just work
}

void Core::GameObject::update(float /* dt */)
{
	// Empty on purpose, allow basic GameObjects to just work
}