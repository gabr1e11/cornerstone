//
// Factory.hpp
//
// @author Roberto Cano
//

#pragma once

#include "EngineOwner.hpp"
#include "ILifeCycle.hpp"

namespace Framework
{
	namespace Core
	{
		class Factory : public EngineOwner, virtual public ILifeCycle
		{
		public:
			Factory() = default;
			virtual ~Factory() = default;

			template<typename T, typename... Args>
			std::shared_ptr<T> CreateComponent(Args&& ...args)
			{
				static_assert(std::is_base_of<GameComponent, T>::value, "Component must inherit from GameComponent");
				auto component = std::make_shared<T>(std::forward<Args>(args)...);
				component->setEngine(getEngine());
				component->internalInit();
				return std::move(component);
			}

			template<typename T, typename... Args>
			std::shared_ptr<T> CreateGameObject(Args&& ...args)
			{
				static_assert(std::is_base_of<GameObject, T>::value, "Class must inherit from GameObject");
				auto gameObject = std::make_shared<T>(std::forward<Args>(args)...);
				gameObject->setEngine(getEngine());
				gameObject->internalInit();
				return std::move(gameObject);
			}
			template<typename T, typename... Args>
			static std::shared_ptr<T> CreateScene(Args&& ...args)
			{
				static_assert(std::is_base_of<Scene, T>::value, "Class must inherit from Scene");
				auto gameObject = std::make_shared<T>(std::forward<Args>(args)...);
				gameObject->internalInit();
				return std::move(gameObject);
			}

		protected:
			void internalInit() override = 0;
			void internalStart() override = 0;
			void internalUpdate(float dt) override = 0;
		};
	}
}