//
// MouseHandler.hpp
//
// @author Roberto Cano
//

#pragma once

#include "Framework/Core/GameComponent.hpp"

namespace Framework
{
	namespace Core
	{
		class GameObject;
	}

	namespace Components
	{
#pragma region - IMouseHandlerDelegate
		//
		// The GameObject using MouseHandler as component must inherit from IMouseHandlerDelegate
		// If an external delegate is set, the external delegate is called first. If the delegate
		// returns true, then the event is allowed to pass further and the internal delegate is triggered
		//
		class IMouseHandlerDelegate
		{
		public:
			using FunctType = bool(IMouseHandlerDelegate::*)(Core::GameObject& gameObject, const Point2D& position);
			virtual ~IMouseHandlerDelegate() = default;

			// Delegate returns true if the movement is allowed
			virtual bool onMouseClicked(Core::GameObject& gameObject, const Point2D& position) = 0;
			virtual bool onMouseDragStarted(Core::GameObject& gameObject, const Point2D& firstPosition) = 0;
			virtual bool onMouseDragContinue(Core::GameObject& gameObject, const Point2D& nextPosition) = 0;
			virtual bool onMouseDragStopped(Core::GameObject& gameObject, const Point2D& lastPosition) = 0;
		};

#pragma region - MouseHandler
		class MouseHandler : public Core::GameComponent
		{
		public:
			MouseHandler(IMouseHandlerDelegate* externalDelegate);
			MouseHandler() = default;

			void update(float dt) override;

		private:
			enum class State
			{
				WaitingForClick,
				MovementDetection,
				OnGameObjectSelectedStart,
				OnGameObjectSelectedContinue,
				OnGameObjectDragged,
				OnGameObjectStopped
			};

			IMouseHandlerDelegate* getExternalDelegate();
			IMouseHandlerDelegate* getInternalDelegate();

			bool checkClickInside();
			void startMovementDetection();
			void startDragDropMovement();
			void continueDragDropMovement();
			void stopDragDropMovement();
			void startClickMovement();
			void stopClickMovement();

			bool callExternalDelegate(IMouseHandlerDelegate::FunctType delegateMethod);
			bool callInternalDelegate(IMouseHandlerDelegate::FunctType delegateMethod);
			bool callDelegate(IMouseHandlerDelegate::FunctType delegateMethod);

			State _state = State::WaitingForClick;
			float _elapsedTimeSinceLastClick = 0.0f;
			Point2D _clickMousePosition;
			bool _wasMouseButtonDown = false;

			Components::IMouseHandlerDelegate* _externalDelegate = nullptr;
		};
	}
}
