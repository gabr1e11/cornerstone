//
// BounceUpDownAnimation.cpp
//
// @author Roberto Cano
//

#include "BounceUpDownAnimation.hpp"

#include "Framework/Core/GameObject.hpp"

using namespace Framework::Components;

namespace BounceUpDownAnimationConstants
{
	const float BouncePercentage = 0.5f;
	const float Acceleration = 1900.0f;
	const float FallExtraOfsset = 25.0f;
}

BounceUpDownAnimation::BounceUpDownAnimation(const Point2D& start, const Point2D& end, float speed)
{
	Config config;

	config.start = start;
	config.end = end;
	config.speed = speed;

	initialize(config);
}

BounceUpDownAnimation::BounceUpDownAnimation(const Config& config)
{
	initialize(config);
}

void BounceUpDownAnimation::initialize(const Config& config)
{
	_config = config;

	_randomGenerator = std::make_unique<RandomGenerator>(0, 200);

	_delay = _randomGenerator->getValue() / 1000.0f;
}

void BounceUpDownAnimation::setOnFinishedCallback(const Callback& onFinished)
{
	_userCallback = onFinished;
}

const BounceUpDownAnimation::Callback& BounceUpDownAnimation::getOnFinishedCallback() const
{
	return _userCallback;
}

void BounceUpDownAnimation::run()
{
	startAnimation();
}

void BounceUpDownAnimation::update(float dt)
{
	switch (_state)
	{
		case State::Stopped:
		{
			// Empty on purpose
			break;
		}
		case State::Delay:
		{
			_animationTime += dt;
			if (_animationTime >= _delay)
			{
				_state = State::Fall;

				_animationTime = 0.0f;
				_startPositionForAnimation = getOwner().getLocalPosition();
			}
			break;
		}
		case State::Fall:
		{
			if (!updateFall(dt))
			{
				_state = State::BounceUp;

				_animationTime = 0.0f;
				_startPositionForAnimation = getOwner().getLocalPosition();
			}
			break;
		}
		case State::BounceUp:
		{
			if (!updateBounceUp(dt))
			{
				_state = State::Finished;
			}
			break;
		}
		case State::Finished:
		{
			stopAnimation();
			break;
		}
	}
}

void BounceUpDownAnimation::cancel()
{
	stopAnimation();
}

void BounceUpDownAnimation::startAnimation()
{
	using namespace BounceUpDownAnimationConstants;

	_animationTime = 0.0f;
	Direction2D pathDirection = _config.end - _config.start;
	_normDirection = glm::normalize(pathDirection);
	_startPositionForAnimation = _config.start;
	_bounceDuration = BouncePercentage * glm::length(pathDirection) / _config.speed;

	_state = State::Delay;
}

void BounceUpDownAnimation::stopAnimation()
{
	_animationTime = 0.0f;

	_state = State::Stopped;

	_userCallback(*this);
}

bool BounceUpDownAnimation::updateBounceUp(float dt)
{
	using namespace BounceUpDownAnimationConstants;

	_animationTime += dt;

	Direction2D velocity = -_normDirection * (4.0f * Acceleration * _animationTime * _animationTime / 2.0f);
	Point2D newPosition = _startPositionForAnimation + velocity;

	if (glm::length(velocity) > glm::length(_config.end - _startPositionForAnimation))
	{
		getOwner().setLocalPosition(_config.end);

		return false;
	}
	else
	{
		getOwner().setLocalPosition(newPosition);

		return true;
	}
}

bool BounceUpDownAnimation::updateFall(float dt)
{
	using namespace BounceUpDownAnimationConstants;

	_animationTime += dt;

	Direction2D velocity = _normDirection * ((-_config.speed * _animationTime) + (Acceleration * _animationTime * _animationTime / 2.0f));
	Point2D newPosition = _startPositionForAnimation + velocity;

	Point2D endPosition = _config.end + FallExtraOfsset * _normDirection;
	if (glm::length(velocity) > glm::length(endPosition - _startPositionForAnimation))
	{
		getOwner().setLocalPosition(endPosition);

		return false;
	}
	else
	{
		getOwner().setLocalPosition(newPosition);

		return true;
	}
}
