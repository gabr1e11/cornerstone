//
// EngineOwner.hpp
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
		class EngineOwner
		{
		public:
			EngineOwner() = default;
			virtual ~EngineOwner() = default;

			virtual void setEngine(Types::Engine::PtrType engine);

			const Types::Engine::PtrType getEngine() const;
			Types::Engine::PtrType getEngine();

		private:
			Types::Engine::PtrType _engine;
		};
	}
}