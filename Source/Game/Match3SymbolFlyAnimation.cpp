//
// Match3SymbolFlyAnimation.cpp
//
// @author Roberto Cano
//

#include "Match3SymbolFlyAnimation.hpp"

#include "Framework/Components/SpriteRenderer.hpp"
#include "Framework/Core/Types.hpp"

#include "Match3Symbol.hpp"
#include "Match3MainScene.hpp"
#include "Match3Globals.hpp"

using namespace Match3::Game;
using namespace Match3;

using Direction2D = Framework::Types::Direction2D;

namespace SymbolFlyAnimationConstants
{
	const float SymbolAnimationSpeed = 700.0f;
	const float SymbolAnimationPhase = 0.2f;
	const float SymbolAnimationAmplitude = 3.0f;
	const float SymbolAnimationFadeLength = 100.0f;
}

SymbolFlyAnimation::SymbolFlyAnimation(Types::Symbol::PtrType symbol, const Point2D& targetPosition, float delay)
	: _symbol(symbol)
	, _targetPosition(targetPosition)
	, _currentPosition(symbol->getWorldPosition())
	, _delay(std::max(delay, 0.0f))
{
	// TODO: randomize phase and speed here
	_phase = 0.0f;
	_speed = SymbolFlyAnimationConstants::SymbolAnimationSpeed;

	symbol->setZIndex(Globals::SymbolFlyingZIndex);
}

void SymbolFlyAnimation::start()
{
	getOwner<MainScene>().addGameObject(_symbol);
	_symbol->setWorldPosition(_currentPosition);
	_symbol->setState(Symbol::State::Flying);
}

void SymbolFlyAnimation::update(float dt)
{
	switch (_state)
	{
		case State::Delay:
		{
			if (!updateWaitDelay(dt))
			{
				_state = State::Animating;
			}
			break;
		}
		case State::Animating:
		{
			if (!updateAnimateSymbol(dt))
			{
				_state = State::Finished;

				getOwner<MainScene>().removeGameObject(*_symbol);
				getOwner<MainScene>().onSymbolFlyAnimationFinished(_symbol);
			}
			break;
		}
		case State::Finished:
		{
			// Empty on purpose
		}
	}
}

bool SymbolFlyAnimation::updateWaitDelay(float dt)
{
	if (_delay <= 0.0f)
	{
		return false;
	}

	_delay -= dt;
	return true;
}

bool SymbolFlyAnimation::updateAnimateSymbol(float dt)
{
	using namespace SymbolFlyAnimationConstants;

	Direction2D animDirection = _targetPosition - _currentPosition;
	float animDirectionLength = glm::length(animDirection);
	if (animDirectionLength < SymbolAnimationFadeLength)
	{
		updateSymbolRenderers(animDirectionLength);
	}
	if (animDirectionLength < (dt * _speed))
	{
		return false;
	}
	else
	{
		Direction2D velocity = dt * _speed * glm::normalize(animDirection);
		Direction2D sineDirection = SymbolAnimationAmplitude * glm::cos(_phase) * Direction2D(velocity.y, -velocity.x);

		_currentPosition += velocity;
		_phase += SymbolAnimationPhase;

		_symbol->setWorldPosition(_currentPosition + sineDirection);

		return true;
	}
}

void SymbolFlyAnimation::updateSymbolRenderers(float animDirectionLength)
{
	using namespace SymbolFlyAnimationConstants;

	const std::set<Framework::Types::InstanceId>& spriteRendererIds = _symbol->getComponentsIds<Framework::Components::SpriteRenderer>();

	float factor = animDirectionLength / SymbolAnimationFadeLength;
	for (const auto& instanceId : spriteRendererIds)
	{
		Framework::Types::SpriteRenderer::PtrType spriteRenderer = _symbol->getComponent<Framework::Components::SpriteRenderer>(instanceId);

		spriteRenderer->setOpacity(factor);
		spriteRenderer->setScale(factor);
	}
}