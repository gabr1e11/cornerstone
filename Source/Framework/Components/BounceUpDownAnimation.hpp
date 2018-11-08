//
// BounceUpDownAnimation.hpp
//
// @author Roberto Cano
//

#pragma once

#include "Framework/Core/GameComponent.hpp"
#include "Framework/Core/Types.hpp"
#include "Framework/Utils/Utils.hpp"

#include <functional>

namespace Framework
{
	namespace Components
	{
		class BounceUpDownAnimation : public Framework::Core::GameComponent
		{
		public:
			using Callback = std::function<void(BounceUpDownAnimation& sender)>;
			using Point2D = Framework::Types::Point2D;

			struct AnimationData
			{
				Point2D start;
				Point2D end;
				float speed = 1.0f;
				float delay = 1.0f;
			};
			using Config = AnimationData;

			BounceUpDownAnimation(const Point2D& start, const Point2D& end, float speed);
			BounceUpDownAnimation(const Config& config);
			~BounceUpDownAnimation() = default;

			void setOnFinishedCallback(const Callback& onFinished);
			const Callback& getOnFinishedCallback() const;

			void run();
			void cancel();

			// GameComponent life cycle
			void update(float dt) override;

		private:
			enum class State
			{
				Stopped,
				Delay,
				Fall,
				BounceUp,
				Finished
			};
			using AnimationState = AnimationData;

			void initialize(const Config& config);

			void startAnimation();
			bool updateBounceUp(float dt);
			bool updateFall(float dt);
			void stopAnimation();

			Config _config;
			Callback _userCallback;
			Types::GameObject::PtrType _owner;

			float _animationTime = 0.0f;
			Point2D _normDirection;
			Point2D _startPositionForAnimation;
			State _state = State::Stopped;
			float _delay = 0.0f;
			float _bounceDuration = 0.0f;

			using RandomGenerator = Framework::Utils::RandomGenerator;
			std::unique_ptr<RandomGenerator> _randomGenerator;
		};
	}
}
