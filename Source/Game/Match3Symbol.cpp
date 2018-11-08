//
// Match3Symbol.cpp
//
// @author Roberto Cano
//

#include "Match3Symbol.hpp"

#include "Match3Globals.hpp"

#include "Framework/Core/Engine.hpp"
#include "Framework/Components/SpriteRenderer.hpp"

using namespace Framework::Core;
using namespace Framework::Types;
using namespace Match3::Game;

namespace SymbolConstants
{
	const Point2D AnchorPoint = Point2D(0.5f, 0.5f);

	const float UpAndDownAnimationPhase = 2.0f * glm::pi<float>() / 2.0f; // rad/sec
	const float UpAndDownAnimationAmplitude = 2.0f;

	const float MoveAnimationInitPhase = 3.0f * glm::pi<float>() / 2.0f;
	const float MoveAnimationPhase = 2.0f * glm::pi<float>() / 1.0f; // rad/sec
	const float MoveAnimationAmplitude = 6.0f;

	const float PossibleMoveAnimationInitPhase = 3.0f * glm::pi<float>() / 2.0f;
	const float PossibleMoveAnimationPhase = 2.0f * glm::pi<float>() / 0.5f; // rad/sec
	const float PossibleMoveAnimationAmplitude = 6.0f;
	const float PossibleMoveAnimationTimeSec = 1.0f;
	const float PossibleMoveAnimationAngleScale = 8.0f;

	const float AppearingAnimationDuration = 0.2f;
	const float AppearingAnimationNumTurns = 1.0f;
}

Symbol::Symbol(Type type, const Size& size)
	: _type(type)
{
	setSize(size);
	setZIndex(Match3::Game::Globals::SymbolNormalZIndex);
	setAnchorPoint(SymbolConstants::AnchorPoint);

	_randomGenerator = std::make_unique<RandomGenerator>(0, 90);

	_upAndDownAnimInitPhase = static_cast<float>(_randomGenerator->getValue()) * 4.0f * glm::pi<float>() / 180.0f;
}

void Symbol::start()
{
	using namespace Framework;

	const std::set<InstanceId>& spriteRenderersIds = getComponentsIds<Components::SpriteRenderer>();
	for (const auto& instanceId : spriteRenderersIds)
	{
		SpriteRendererPtr spriteRenderer = getComponent<Components::SpriteRenderer>(instanceId);
		if (spriteRenderer->getName() == Match3::Game::Globals::SymbolSpriteRendererName)
		{
			_symbolSpriteRenderer = std::move(spriteRenderer);
		}
		else if (spriteRenderer->getName() == Match3::Game::Globals::ShadowSpriteRendererName)
		{
			_shadowSpriteRenderer = std::move(spriteRenderer);
		}
	}

	assert(_symbolSpriteRenderer && _shadowSpriteRenderer);
}

void Symbol::update(float dt)
{
	if (_state != _newState)
	{
		processStateChange(_newState);
		return;
	}

	switch (_state)
	{
		case State::Idle:
		{
			updateUpAndDownAnimation(dt);
			break;
		}
		case State::Appearing:
		{
			if (!updateAppearingAnimation(dt))
			{
				_newState = State::Idle;
			}
			break;
		}
		case State::Selected:
		{
			updateSelectedAnimation(dt);
			break;
		}
		case State::PossibleMove:
		{
			updatePossibleMoveAnimation(dt);
			break;
		}
		case State::Dropping:
		case State::Flying:
		{
			// Empty on purpose
			break;
		}
	}
}

Symbol::Type Symbol::getType() const
{
	return _type;
}

void Symbol::setCellPosition(const CellPosition& cellPosition)
{
	_cellPosition = cellPosition;

	updatePositionFromCellPosition();
}

const Symbol::CellPosition& Symbol::getCellPosition() const
{
	return _cellPosition;
}

Point2D Symbol::getLocalPositionFromCellPosition() const
{
	Point2D position = Point2D(_cellPosition.x * getSize().x, _cellPosition.y * getSize().y);
	position += getAnchorPointLocalPosition();
	return position;
}

void Symbol::setState(State state)
{
	_newState = state;
}

void Symbol::processStateChange(State newState)
{
	// TODO: Need a proper state machine here to handle exit state
	switch (_state)
	{
		case State::Idle:
		{
			stopUpAndDownAnimation();
			break;
		}
		case State::Appearing:
		{
			stopAppearingAnimation();
			break;
		}
		case State::Selected:
		{
			stopSelectedAnimation();
			break;
		}
		case State::PossibleMove:
		{
			stopPossibleMoveAnimation();
			break;
		}
		case State::Dropping:
		case State::Flying:
		{
			// Empty on purpose
			break;
		}
	}

	_state = newState;

	// TODO: Same here for enter state
	switch (_state)
	{
		case State::Idle:
		{
			startUpAndDownAnimation();
			break;
		}
		case State::Appearing:
		{
			startAppearingAnimation();
			break;
		}
		case State::Selected:
		{
			startSelectedAnimation();
			break;
		}
		case State::PossibleMove:
		{
			startPossibleMoveAnimation();
			break;
		}
		case State::Dropping:
		case State::Flying:
		{
			// Empty on purpose
			break;
		}
	}
}

Symbol::State Symbol::getState() const
{
	return _state;
}

void Symbol::updatePositionFromCellPosition()
{
	Point2D position = getLocalPositionFromCellPosition();
	setLocalPosition(position);
}

bool Symbol::onMouseClicked(GameObject& gameObject, const Point2D& position)
{
	// Empty on purpose, state for click must be se manually by the board
	return true;
}

bool Symbol::onMouseDragStarted(GameObject& gameObject, const Point2D& firstPosition)
{
	assert(_state == State::Idle);

	setZIndex(Match3::Game::Globals::SymbolOnTopZIndex);
	setWorldPosition(firstPosition);
	return true;
}

bool Symbol::onMouseDragContinue(GameObject& gameObject, const Point2D& nextPosition)
{
	setWorldPosition(nextPosition);
	return true;
}

bool Symbol::onMouseDragStopped(GameObject& gameObject, const Point2D& lastPosition)
{
	setZIndex(Match3::Game::Globals::SymbolNormalZIndex);
	setWorldPosition(lastPosition);
	return true;
}

void Symbol::startSelectedAnimation()
{
	// TODO: animation system
	_animationTime = 0.0f;
	setZIndex(Match3::Game::Globals::SymbolOnTopZIndex);
}

void Symbol::updateSelectedAnimation(float dt)
{
	using namespace SymbolConstants;

	_animationTime += dt;

	Point2D position = getLocalPositionFromCellPosition();
	position.y -= MoveAnimationAmplitude * (1.0f + glm::sin(MoveAnimationInitPhase + _animationTime * MoveAnimationPhase));

	setLocalPosition(position);
}

void Symbol::stopSelectedAnimation()
{
	updatePositionFromCellPosition();
	setZIndex(Match3::Game::Globals::SymbolNormalZIndex);
}

void Symbol::startPossibleMoveAnimation()
{
	_animationTime = 0.0;
}

void Symbol::updatePossibleMoveAnimation(float dt)
{
	using namespace SymbolConstants;

	_animationTime += dt;

	float PI2 = glm::pi<float>();
	float scaler = glm::sin(_animationTime * PI2 / PossibleMoveAnimationTimeSec);
	float rotation = scaler * PI2 / PossibleMoveAnimationAngleScale;

	_symbolSpriteRenderer->setRotation(rotation);

	Point2D position = getLocalPositionFromCellPosition();
	position.y -= PossibleMoveAnimationAmplitude * (1.0f + glm::sin(PossibleMoveAnimationInitPhase + _animationTime * PossibleMoveAnimationPhase)) / 2.0f;

	setLocalPosition(position);
}

void Symbol::stopPossibleMoveAnimation()
{
	_symbolSpriteRenderer->setRotation(0.0f);
	updatePositionFromCellPosition();
}

void Symbol::startAppearingAnimation()
{
	_animationTime = 0.0f;

	setRendererVisible(true);
	setRendererRotation(0.0f);
	setRendererOpacity(0.0f);
	setRendererScale(0.0f);
}

bool Symbol::updateAppearingAnimation(float dt)
{
	using namespace SymbolConstants;

	_animationTime += dt;
	if (_animationTime > AppearingAnimationDuration)
	{
		stopAppearingAnimation();
		return false;
	}

	float animationPercent = _animationTime / AppearingAnimationDuration;

	float rotation = AppearingAnimationNumTurns * animationPercent * 2.0f * glm::pi<float>();

	setRendererRotation(rotation);
	setRendererOpacity(animationPercent);
	setRendererScale(animationPercent);

	updatePositionFromCellPosition();

	return true;
}

void Symbol::stopAppearingAnimation()
{
	setRendererRotation(0.0f);
	setRendererOpacity(1.0f);
	setRendererScale(1.0f);
}

void Symbol::setRendererVisible(bool visible)
{
	_symbolSpriteRenderer->setVisible(true);
	_shadowSpriteRenderer->setVisible(true);
}

void Symbol::setRendererOpacity(float opacity)
{
	_symbolSpriteRenderer->setOpacity(opacity);
	_shadowSpriteRenderer->setOpacity(opacity);
}

void Symbol::setRendererRotation(float rotation)
{
	_symbolSpriteRenderer->setRotation(rotation);
	_shadowSpriteRenderer->setRotation(rotation);
}

void Symbol::setRendererScale(float scale)
{
	_symbolSpriteRenderer->setOpacity(scale);
	_shadowSpriteRenderer->setOpacity(scale);
}

void Symbol::startUpAndDownAnimation()
{
	_animationTime = 0.0f;
}

void Symbol::updateUpAndDownAnimation(float dt)
{
	using namespace SymbolConstants;

	_animationTime += dt;

	Point2D position = getLocalPositionFromCellPosition();
	position.y -= UpAndDownAnimationAmplitude * glm::sin(_upAndDownAnimInitPhase + _animationTime * UpAndDownAnimationPhase);

	setLocalPosition(position);
}

void Symbol::stopUpAndDownAnimation()
{
	_animationTime = 0.0f;
}