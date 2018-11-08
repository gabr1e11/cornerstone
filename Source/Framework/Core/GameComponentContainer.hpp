//
// GameComponentContainer.hpp
//
// @author Roberto Cano
//

#pragma once

#include <set>
#include <queue>

#include "Types.hpp"

#include "GameComponent.hpp"
#include "Framework/Utils/Utils.hpp"

namespace Framework
{
	namespace Core
	{
		class GameObject;
	}
}

namespace Framework
{
	using namespace Types;
	namespace Core
	{
		class GameComponentContainer : virtual public ILifeCycle, public std::enable_shared_from_this<GameObject>
		{
		public:
			GameComponentContainer() = default;
			virtual ~GameComponentContainer() = default;

			virtual void addComponent(Types::GameComponent::PtrType component);

			bool removeComponent(const GameComponent& component);
			bool removeComponent(const InstanceId& instanceId);

			bool hasComponent(const GameComponent& component) const;
			bool hasComponent(const InstanceId& componentId) const;
			bool hasComponents(const ComponentId& componentId) const;

			template<typename T>
			std::shared_ptr<T> getComponent(const InstanceId& instanceId);
			template<typename T>
			std::shared_ptr<const T> getComponent(const InstanceId& instanceId) const;
			const std::set<InstanceId>& getComponentsIds(const ComponentId& componentId) const;
			template<typename T>
			const std::set<InstanceId>& getComponentsIds() const;

			// Works if only there is 1 component of the given type
			template<typename T>
			std::shared_ptr<T> getComponent();
			template<typename T>
			std::shared_ptr<const T> getComponent() const;

			void init() override = 0;
			void start() override = 0;
			void update(float dt) = 0;

		protected:
			void internalInit() override;
			void internalStart() override;
			void internalUpdate(float dt) override;

		private:
			void updateNotStarted();

			std::queue<Types::GameComponent::WeakPtrType> _notStartedComponents;
			std::unordered_map<InstanceId, Types::GameComponent::PtrType> _componentsMapByInstanceId;
			std::unordered_map<ComponentId, std::set<InstanceId>> _componentsMapByComponentId;
		};

#include "GameComponentContainer.inl.hpp"
	}
}