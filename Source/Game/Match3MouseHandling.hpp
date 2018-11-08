//
// Match3MouseHandling.hpp
//
// @author Roberto Cano
//

#pragma once

#include "Match3MouseHandling.fwd.hpp"

#include "Framework/Components/MouseHandler.hpp"
#include "Framework/Core/GameObject.hpp"
#include "Framework/Core/Types.hpp"

#include "Match3Symbol.fwd.hpp"
#include "Match3Board.fwd.hpp"

namespace Match3
{
	namespace Game
	{
		class MouseHandling final : public Framework::Components::MouseHandler, public Framework::Components::IMouseHandlerDelegate
		{
		public:
			using GameObject = Framework::Core::GameObject;
			using Point2D = Framework::Types::Point2D;

			MouseHandling();

			void update(float dt) override;

			bool onMouseClicked(GameObject& gameObject, const Point2D& position) override;
			bool onMouseDragStarted(GameObject& gameObject, const Point2D& firstPosition) override;
			bool onMouseDragContinue(GameObject& gameObject, const Point2D& nextPosition) override;
			bool onMouseDragStopped(GameObject& gameObject, const Point2D& lastPosition) override;

		private:
			enum class State
			{
				Idle,
				MovementDetection,
				OnGameObjectSelected,
				OnGameObjectDragged,
				OnGameObjectStopped,
			};

			State _state = State::Idle;

			Types::Symbol::PtrType _selectedSymbol;
		};
	}
}
