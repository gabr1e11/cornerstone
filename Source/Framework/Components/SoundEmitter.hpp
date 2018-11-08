//
// SoundEmitter.hpp
//
// @author Roberto Cano
//

#pragma once

#include "Framework/Core/GameComponent.hpp"

namespace Framework
{
	namespace Components
	{
		class SoundEmitter : public Core::GameComponent
		{
		public:
			SoundEmitter() = default;
			~SoundEmitter() override = default;

			void setVisualDescription(const std::string& description);
			const std::string& getVisualDescription() const;

			void setRotation(float rotation);
			float getRotation() const;

			void play();

			void update(float dt) override;

		private:
			enum class State
			{
				Stopped,
				Playing
			};

			bool updateVisualDescriptionAnimation(float dt);

			std::string _visualDescription;

			float _animationTime = 0.0f;
			float _rotation = 0.0f;
			float _currentRotation = 0.0f;
			State _state = State::Stopped;
			State _newState = State::Stopped;
		};
	}
}
