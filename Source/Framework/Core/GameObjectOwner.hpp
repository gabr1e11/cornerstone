//
// GameObjectOwner.hpp
//
// @author Roberto Cano
//

#pragma once

#include "Types.hpp"

namespace Framework
{
	using namespace Types;
	namespace Core
	{
		class GameObjectOwner
		{
		public:
			GameObjectOwner() = default;
			virtual ~GameObjectOwner() = default;

			bool hasOwner() const;

			const GameObject& getOwner() const;
			GameObject& getOwner();
			template<typename T>
			const T& getOwner() const
			{
				static_assert(std::is_base_of<GameObject, T>::value, "Object must inherit from GameObject");
				return dynamic_cast<const T&>(getOwner());
			}
			template<typename T>
			T& getOwner()
			{
				static_assert(std::is_base_of<GameObject, T>::value, "Object must inherit from GameObject");
				return dynamic_cast<T&>(getOwner());
			}

			virtual void _setOwner(Types::GameObject::WeakPtrType gameObject);

		private:
			Types::GameObject::WeakPtrType _gameObject;
		};
	}
}