//
// GameObjectComponent.hpp
//
// @author Roberto Cano
//

#pragma once

#include "Instanceable.hpp"
#include "EngineOwner.hpp"
#include "GameObjectOwner.hpp"
#include "Factory.hpp"

// Forward declarations
namespace Framework
{
	namespace Core
	{
		class GameObject;
	}
}

namespace Framework
{
	namespace Core
	{
		class GameComponent : public Instanceable, public GameObjectOwner, public Factory
		{
		public:
			virtual ~GameComponent() = default;

			const ComponentId& getComponentId() const;
			using Instanceable::getInstanceId;

			void init() override;
			void start() override;
			void update(float dt) override;

		protected:
			friend class GameComponentContainer;
			friend class Factory;
			void internalInit() override;
			void internalStart() override;
			void internalUpdate(float dt) override;

			GameComponent();

		private:
			ComponentId _componentId;
		};

	}
}