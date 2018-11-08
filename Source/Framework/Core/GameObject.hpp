//
// GameObject.hpp
//
// @author Roberto Cano
//

#pragma once

#include "Types.hpp"
#include "Instanceable.hpp"
#include "GameComponentContainer.hpp"
#include "GameObjectContainer.hpp"
#include "Factory.hpp"
#include "EngineOwner.hpp"
#include "AnchorPointUser.hpp"

namespace Framework
{
	using namespace Types;
	namespace Core
	{
		class GameObject : public Instanceable, public GameComponentContainer, public GameObjectContainer,
			public AnchorPointUser, public Factory, public GameObjectOwner
		{
		public:
			GameObject() = default;

			void setWorldPosition(const Point2D& newPosition);
			const Point2D& getWorldPosition() const;
			void setLocalPosition(const Point2D& newPosition);
			const Point2D& getLocalPosition() const;

			Point2D getAnchorPointLocalPosition() const;

			void setZIndex(int zIndex);
			int getZIndex() const;

			void init() override;
			void start() override;
			void update(float dt) override;

			void addGameObject(Types::GameObject::PtrType gameObject) override;

			void setSize(const Size2D& size);
			const Size2D& getSize() const;

			bool isPointInside(const Point2D& point) const;

		protected:
			friend class Framework::Core::Engine;
			friend class Framework::Core::GameObjectContainer;
			friend class Framework::Core::Factory;

			void internalInit() override;
			void internalStart() override;
			void internalUpdate(float dt) override;

			void _setOwner(Types::GameObject::WeakPtrType gameObject) override;

		private:
			void updateLocalPosition(const Point2D& newPosition);
			void updateWorldPosition(const Point2D& newPosition);

			Types::GameObject::WeakPtrType _parentObject;
			Point2D _worldPosition;
			Point2D _localPosition;
			Size2D _size;
			int _zIndex = 0;
		};
	}
}
