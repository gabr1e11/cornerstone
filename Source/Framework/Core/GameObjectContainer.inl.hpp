//
// GameObjectContainer.inl.hpp
//
// @author Roberto Cano
//


template<typename T>
std::shared_ptr<T> GameObjectContainer::getGameObject(const Types::InstanceId& instanceId)
{
	return const_cast<T&>(static_cast<const GameObjectContainer*>(this)->getGameObject<T>(instanceId));
}

template<typename T>
std::shared_ptr<const T> GameObjectContainer::getGameObject(const Types::InstanceId& instanceId) const
{
	auto goMapIter = _gameObjects.find(instanceId);
	assert(goMapIter != _gameObjects.end());

	return std::dynamic_pointer_cast<T>(goMapIter->second);
}