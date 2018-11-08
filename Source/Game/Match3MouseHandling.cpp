//
// Match3MouseHandling.cpp
//
// @author Roberto Cano
//

#include "Match3MouseHandling.hpp"

#include "Framework/Core/Engine.hpp"
#include "Match3Symbol.hpp"
#include "Match3Board.hpp"
#include "Match3Cell.hpp"

using namespace Match3::Game;

MouseHandling::MouseHandling()
	: MouseHandler(this)
{
	// Empty on purpose
}

void MouseHandling::update(float dt)
{
	MouseHandler::update(dt);

	switch (_state)
	{
		case State::OnGameObjectStopped:
		{
			if (!getEngine()->isMouseButtonDown())
			{
				_state = State::Idle;
			}
			break;
		}
	}
}

bool MouseHandling::onMouseDragStarted(GameObject& gameObject, const Point2D& firstPosition)
{
	Board& owner = getOwner<Board>();

	if (_state != State::Idle || !owner.isUserInteractionEnabled())
	{
		return false;
	}

	_selectedSymbol = owner.getSymbolFromWorldPosition(firstPosition);
	if (_selectedSymbol == nullptr)
	{
		return false;
	}

	_state = State::OnGameObjectDragged;

	_selectedSymbol->onMouseDragStarted(gameObject, firstPosition);
	owner.onUserSelectedSymbol(_selectedSymbol, true);

	return true;
}

bool MouseHandling::onMouseDragContinue(GameObject& gameObject, const Point2D& nextPosition)
{
	if (_state != State::OnGameObjectDragged)
	{
		return false;
	}

	_selectedSymbol->onMouseDragContinue(gameObject, nextPosition);

	return true;
}

bool MouseHandling::onMouseDragStopped(GameObject& gameObject, const Point2D& lastPosition)
{
	if (_state != State::OnGameObjectDragged)
	{
		return false;
	}

	_state = State::Idle;

	_selectedSymbol->onMouseDragStopped(gameObject, lastPosition);

	getOwner<Board>().onUserPerformedMove(_selectedSymbol, lastPosition);

	_selectedSymbol = nullptr;

	return true;
}

bool MouseHandling::onMouseClicked(GameObject& gameObject, const Point2D& position)
{
	Board& owner = getOwner<Board>();

	if (!owner.isUserInteractionEnabled())
	{
		return false;
	}

	switch (_state)
	{
		case State::Idle:
		{
			_selectedSymbol = owner.getSymbolFromWorldPosition(position);
			if (_selectedSymbol)
			{
				_state = State::OnGameObjectSelected;

				_selectedSymbol->onMouseClicked(gameObject, position);
				owner.onUserSelectedSymbol(_selectedSymbol, false);
			}
			return true;
		}
		case State::OnGameObjectSelected:
		{
			_state = State::OnGameObjectStopped;

			owner.onUserPerformedMove(_selectedSymbol, position);

			_selectedSymbol = nullptr;

			return true;
		}
	}
	return false;
}