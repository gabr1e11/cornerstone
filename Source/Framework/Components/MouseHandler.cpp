//
// MouseHandler.cpp
//
// @author Roberto Cano
//

#include "MouseHandler.hpp"

#include "Framework/Core/Engine.hpp"
#include "Framework/Core/GameObject.hpp"

using namespace Framework::Components;
using namespace Framework::Core;

namespace MouseHandlerConstants
{
	const float ClickAndReleaseTimeSec = 0.3f; // 300 milliseconds
	const float DragAndDropThreshold = 2.0f;
}

MouseHandler::MouseHandler(IMouseHandlerDelegate* externalDelegate)
	: _externalDelegate(externalDelegate)
{
	// Empty on purpose
}

IMouseHandlerDelegate* MouseHandler::getExternalDelegate()
{
	return _externalDelegate;
}

IMouseHandlerDelegate* MouseHandler::getInternalDelegate()
{
	Core::GameObject& gameObject = getOwner();
	IMouseHandlerDelegate* delegate = dynamic_cast<IMouseHandlerDelegate*>(&gameObject);

	return delegate;
}

void MouseHandler::update(float dt)
{
	bool isMouseClicked = getEngine()->isMouseButtonDown();

	switch (_state)
	{
		case State::WaitingForClick:
		{
			if (checkClickInside())
			{
				startMovementDetection();
				_elapsedTimeSinceLastClick = 0.0f;
				_clickMousePosition = getEngine()->getMousePosition();

				_state = State::MovementDetection;
			}
			break;
		}
		case State::MovementDetection:
		{
			_elapsedTimeSinceLastClick += dt;

			if (isMouseClicked)
			{
				const Point2D mousePosition = getEngine()->getMousePosition();
				const float mouseMovementLength = glm::length(mousePosition - _clickMousePosition);

				if (mouseMovementLength > MouseHandlerConstants::DragAndDropThreshold)
				{
					startDragDropMovement();
				}
			}
			else if (_elapsedTimeSinceLastClick < MouseHandlerConstants::ClickAndReleaseTimeSec)
			{
				startClickMovement();
			}
			else
			{
				_state = State::WaitingForClick;
			}
			break;
		}
		case State::OnGameObjectSelectedStart:
		{
			_state = State::OnGameObjectSelectedContinue;
			break;
		}
		case State::OnGameObjectSelectedContinue:
		{
			if (isMouseClicked)
			{
				stopClickMovement();
			}
			break;
		}
		case State::OnGameObjectDragged:
		{
			if (!isMouseClicked)
			{
				stopDragDropMovement();
			}
			else
			{
				continueDragDropMovement();
			}
			break;
		}
		case State::OnGameObjectStopped:
		{
			if (!isMouseClicked)
			{
				_state = State::WaitingForClick;
			}
		}
	}
}

bool MouseHandler::checkClickInside()
{
	bool isMouseButtonDown = getEngine()->isMouseButtonDown();

	if (_wasMouseButtonDown && !isMouseButtonDown)
	{
		_wasMouseButtonDown = false;
	}
	if (isMouseButtonDown)
	{
		_wasMouseButtonDown = true;
	}
	else
	{
		return false;
	}

	const Point2D mousePosition = getEngine()->getMousePosition();
	return getOwner().isPointInside(mousePosition);
}

void  MouseHandler::startMovementDetection()
{
	_state = State::MovementDetection;

	_elapsedTimeSinceLastClick = 0.0;
	_clickMousePosition = getEngine()->getMousePosition();
}

void MouseHandler::startDragDropMovement()
{
	if (callDelegate(&IMouseHandlerDelegate::onMouseDragStarted))
	{
		_state = State::OnGameObjectDragged;
	}
	else
	{
		_state = State::MovementDetection;
	}
}

void MouseHandler::continueDragDropMovement()
{
	callDelegate(&IMouseHandlerDelegate::onMouseDragContinue);
}

void MouseHandler::stopDragDropMovement()
{
	_state = State::OnGameObjectStopped;

	callDelegate(&IMouseHandlerDelegate::onMouseDragStopped);
}

void MouseHandler::startClickMovement()
{
	if (callDelegate(&IMouseHandlerDelegate::onMouseClicked))
	{
		_state = State::OnGameObjectSelectedStart;
	}
	else
	{
		_state = State::MovementDetection;
	}
}

void MouseHandler::stopClickMovement()
{
	_state = State::OnGameObjectStopped;
	callDelegate(&IMouseHandlerDelegate::onMouseClicked);
}

bool MouseHandler::callExternalDelegate(IMouseHandlerDelegate::FunctType delegateMethod)
{
	IMouseHandlerDelegate* externalDelegate = getExternalDelegate();
	if (externalDelegate == nullptr)
	{
		return true;
	}

	Core::GameObject& parentObject = getOwner();
	Point2D mousePosition = getEngine()->getMousePosition();


	return (externalDelegate->*delegateMethod)(parentObject, mousePosition);
}

bool MouseHandler::callInternalDelegate(IMouseHandlerDelegate::FunctType delegateMethod)
{
	IMouseHandlerDelegate* internalDelegate = getInternalDelegate();
	if (internalDelegate == nullptr)
	{
		return true;
	}

	Core::GameObject& parentObject = getOwner();
	Point2D mousePosition = getEngine()->getMousePosition();

	return (internalDelegate->*delegateMethod)(parentObject, mousePosition);
}

bool MouseHandler::callDelegate(IMouseHandlerDelegate::FunctType delegateMethod)
{
	if (callExternalDelegate(delegateMethod))
	{
		return callInternalDelegate(delegateMethod);
	}
	else
	{
		return false;
	}
}