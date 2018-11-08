//
// GameObjectContainer.hpp
//
// @author Roberto Cano
//

#pragma once

#include <queue>
#include <map>

#include "Types.hpp"
#include "ILifeCycle.hpp"

namespace Framework
{
	using namespace Types;
	namespace Core
	{
		class GameObjectContainer : virtual public ILifeCycle
		{
		public:
			GameObjectContainer() = default;
			virtual ~GameObjectContainer() = default;

			virtual void addGameObject(Types::GameObject::PtrType gameObject);

			bool hasGameObject(const GameObject& gameObject) const;
			bool hasGameObject(const InstanceId& instanceId) const;

			template<typename T>
			std::shared_ptr<T> getGameObject(const Types::InstanceId& instanceId);
			template<typename T>
			std::shared_ptr<const T> getGameObject(const Types::InstanceId& instanceId) const;

			void removeGameObject(const GameObject& gameObject);
			void removeGameObject(const InstanceId& instanceId);

			void init() override = 0;
			void start() override = 0;
			void update(float dt) override = 0;

		protected:
			void internalInit() override;
			void internalStart() override;
			void internalUpdate(float dt) override;

		private:
			void updateNotStarted();
			void doAddGameObject(Types::GameObject::PtrType gameObject);

			std::queue<Types::GameObject::WeakPtrType> _notStartedObjects;
			std::unordered_map<InstanceId, Types::GameObject::PtrType> _gameObjects;
		};

#include "GameObjectContainer.inl.hpp"
	}
}

