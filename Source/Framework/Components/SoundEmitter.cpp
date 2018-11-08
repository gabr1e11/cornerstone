//
// SoundEmitter.cpp
//
// @author Roberto Cano
//

#include "SoundEmitter.hpp"

#include "Framework/Core/Engine.hpp"
#include "Framework/Core/GameObject.hpp"
#include "Framework/Core/Types.hpp"

using namespace Framework::Components;

namespace SoundEmitterConstants
{
	const float AnimationDuration = 0.4f;
	const float FinalScale = 0.4f;
	const Framework::Types::Color3D TextColor(1.0f, 0.3f, 0.6);
	const Framework::Types::Point2D Offset(50.0f, -25.0f);
}

void SoundEmitter::setVisualDescription(const std::string& description)
{
	_visualDescription = description;
}

const std::string& SoundEmitter::getVisualDescription() const
{
	return _visualDescription;
}

void Framework::Components::SoundEmitter::setRotation(float rotation)
{
	_rotation = rotation;
}

float Framework::Components::SoundEmitter::getRotation() const
{
	return _rotation;
}

void SoundEmitter::play()
{
	_newState = State::Playing;
}

void SoundEmitter::update(float dt)
{
	if (_newState != _state)
	{
		_state = _newState;

		if (_state == State::Playing)
		{
			_currentRotation = _rotation;
		}
	}

	switch (_state)
	{
		case State::Stopped:
		{
			// Empty on purpose
			break;
		}
		case State::Playing:
		{
			if (!updateVisualDescriptionAnimation(dt))
			{
				_newState = State::Stopped;
			}
			break;
		}
	}
}

bool SoundEmitter::updateVisualDescriptionAnimation(float dt)
{
	_animationTime += dt;

	if (_animationTime > SoundEmitterConstants::AnimationDuration)
	{
		_animationTime = 0.0f;
		return false;
	}

	Framework::Core::GameObject& owner = getOwner<Framework::Core::GameObject>();
	float factor = _animationTime / (SoundEmitterConstants::AnimationDuration / 2.5f);

	if (factor > 1.5f)
	{
		factor = 2.5f - factor;
	}
	else if (factor > 1.0f)
	{
		factor = 1.0f;
	}

	Point2D worldPosition = owner.getWorldPosition() + SoundEmitterConstants::Offset;

	getEngine()->renderText(_visualDescription, worldPosition, owner.getZIndex() + 1, SoundEmitterConstants::TextColor,
		_currentRotation, factor * SoundEmitterConstants::FinalScale, factor);

	return true;
}