//
// Match3SymbolFlyAnimation.hpp
//
// @author Roberto Cano
//

#pragma once

#include "Match3SymbolFlyAnimation.fwd.hpp"

#include "Framework/Core/GameObject.hpp"

#include "Match3Symbol.fwd.hpp"

namespace Match3
{
	namespace Game
	{
		class SymbolFlyAnimation final : public Framework::Core::GameObject
		{
		public:
			using Point2D = Framework::Types::Point2D;

			SymbolFlyAnimation(Types::Symbol::PtrType symbol, const Point2D& targetPosition, float delay);

			void start() override;
			void update(float dt) override;

		private:
			enum class State
			{
				Delay,
				Animating,
				Cleanup,
				Finished
			};

			bool updateWaitDelay(float dt);
			bool updateAnimateSymbol(float dt);
			void updateSymbolRenderers(float animDirectionLength);

			State _state = State::Delay;

			Types::Symbol::PtrType _symbol;
			Point2D _currentPosition;
			Point2D _targetPosition;
			float _delay = 0.0f;
			float _phase = 0.0f;
			float _speed = 0.0f;
		};
	}
}

