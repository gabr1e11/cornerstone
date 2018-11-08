//
// Scene.hpp
//
// @author Roberto Cano
//

#pragma once

#include "GameObject.hpp"

namespace Framework
{
	using namespace Types;
	namespace Core
	{
		class Scene : public GameObject
		{
		public:
			using Size2D = Framework::Types::Size2D;

			Scene(const Size2D& sceneSize);
		};
	}
}