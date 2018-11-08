//
// GameComponentContainer.inl.hpp
//
// @author Roberto Cano
//

template<typename T>
std::shared_ptr<T> GameComponentContainer::getComponent(const InstanceId& instanceId)
{
	auto instancesMapIter = _componentsMapByInstanceId.find(instanceId);
	assert(instancesMapIter != _componentsMapByInstanceId.end());
	return std::dynamic_pointer_cast<T>(instancesMapIter->second);
}

template<typename T>
std::shared_ptr<const T> GameComponentContainer::getComponent(const InstanceId& instanceId) const
{
	auto instancesMapIter = _componentsMapByInstanceId.find(instanceId);
	assert(instancesMapIter != _componentsMapByInstanceId.end());
	return std::dynamic_pointer_cast<const T>(instancesMapIter->second);
}

template<typename T>
std::shared_ptr<T> GameComponentContainer::getComponent()
{
	const std::string& componentId = Framework::Utils::GetComponentIdFromClass<T>();
	const std::set<InstanceId>& instancesIds = getComponentsIds(componentId);
	assert(instancesIds.size() == 1);

	const InstanceId& instanceId = *instancesIds.begin();
	return getComponent<T>(instanceId);
}

template<typename T>
std::shared_ptr<const T> GameComponentContainer::getComponent() const
{
	const std::string& componentId = Framework::Utils::GetComponentIdFromClass<T>();
	const std::set<InstanceId>& instancesIds = getComponentsIds(componentId);
	assert(instancesIds.size() == 1);

	const InstanceId& instanceId = *instancesIds.begin();
	return getComponent<T>(instanceId);
}

template<typename T>
const std::set<InstanceId>& GameComponentContainer::getComponentsIds() const
{
	const std::string& componentId = Framework::Utils::GetComponentIdFromClass<T>();
	return getComponentsIds(componentId);
}