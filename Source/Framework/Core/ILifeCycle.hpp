//
// ILifeCycle.hpp
//
// @author Roberto Cano
//

#pragma once

namespace Framework
{
	namespace Core
	{
		class ILifeCycle
		{
		public:
			virtual void init() = 0;
			virtual void start() = 0;
			virtual void update(float dt) = 0;

		protected:
			virtual void internalInit() = 0;
			virtual void internalStart() = 0;
			virtual void internalUpdate(float dt) = 0;
		};
	}
}
