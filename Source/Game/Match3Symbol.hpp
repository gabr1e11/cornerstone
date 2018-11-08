//
// Match3Symbol.hpp
//
// @author Roberto Cano
//

#pragma once

#include "Match3Symbol.fwd.hpp"

#include "Match3Cell.fwd.hpp"

#include "Framework/Core/Types.hpp"
#include "Framework/Core/GameObject.hpp"
#include "Framework/Components/MouseHandler.hpp"
#include "Framework/Utils/Utils.hpp"

namespace Match3
{
	namespace Game
	{
		class Symbol final : public Framework::Core::GameObject, public Framework::Components::IMouseHandlerDelegate
		{
		public:
			using Size = Framework::Types::Size;
			using Type = Match3::Types::Symbol::Type;
			using CellPosition = Match3::Types::Cell::Position;

			enum class State
			{
				Idle,
				Appearing,
				Selected,
				PossibleMove,
				Dropping,
				Flying,
			};

			Symbol(Type type, const Size& size);

			void start() override;
			void update(float dt) override;

			Type getType() const;

			void setCellPosition(const CellPosition& cellPosition);
			const CellPosition& getCellPosition() const;

			void setState(State state);
			State getState() const;

		private:
			using SpriteRendererPtr = Framework::Types::SpriteRenderer::PtrType;

			void processStateChange(State newState);

			friend class MouseHandling; // Only MouseHandling can send mouse events
			bool onMouseClicked(GameObject& gameObject, const Point2D& position) override;
			bool onMouseDragStarted(GameObject& gameObject, const Point2D& firstPosition) override;
			bool onMouseDragContinue(GameObject& gameObject, const Point2D& nextPosition) override;
			bool onMouseDragStopped(GameObject& gameObject, const Point2D& lastPosition) override;

			void setRendererVisible(bool visible);
			void setRendererOpacity(float opacity);
			void setRendererRotation(float rotation);
			void setRendererScale(float scale);

			Point2D getLocalPositionFromCellPosition() const;
			void updatePositionFromCellPosition();

			void startSelectedAnimation();
			void updateSelectedAnimation(float dt);
			void stopSelectedAnimation();

			void startPossibleMoveAnimation();
			void updatePossibleMoveAnimation(float dt);
			void stopPossibleMoveAnimation();

			void startAppearingAnimation();
			bool updateAppearingAnimation(float dt);
			void stopAppearingAnimation();

			void startUpAndDownAnimation();
			void updateUpAndDownAnimation(float dt);
			void stopUpAndDownAnimation();

			CellPosition _cellPosition;
			Type _type = Type::_end;
			State _state = State::Idle;
			State _newState = State::Idle;

			SpriteRendererPtr _symbolSpriteRenderer;
			SpriteRendererPtr _shadowSpriteRenderer;

			float _upAndDownAnimInitPhase = 0.0f;
			float _animationTime = 0.0f;

			using RandomGenerator = Framework::Utils::RandomGenerator;
			std::unique_ptr<RandomGenerator> _randomGenerator;
		};

	}
}
