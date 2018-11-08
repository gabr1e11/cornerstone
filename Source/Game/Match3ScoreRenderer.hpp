//
// Match3ScoreRenderer.cpp
//
// @author Roberto Cano
//

#pragma once

#include "Framework/Core/GameObject.hpp"
#include "Framework/Core/Types.hpp"
#include "Framework/Utils/Utils.hpp"

#include "Match3Factory.fwd.hpp"

namespace Match3
{
	namespace Game
	{
		class ScoreRenderer final : public Framework::Core::GameObject
		{
		public:
			ScoreRenderer() = default;
			~ScoreRenderer() override = default;

			void setPrefix(const std::string& prefix);
			const std::string& getPrefix() const;

			void addScore(int score);
			void resetScore();

			void setSoundEnabled(bool enabled);
			bool isSoundEnabled() const;
			void start() override;

		private:
			void setupRandomGenerator();
			void setupFactory();
			void setupTextRenderer();
			void setupSoundEmitter();

			void updateTextRenderer();
			void triggerSoundEmitter();

			int _score = 0;
			std::string _prefix;
			bool _isSoundEnabled = true;

			Types::Factory::PtrType _factory;
			Framework::Types::SoundEmitter::PtrType _soundEmitter;
			Framework::Types::TextRenderer::PtrType _textRenderer;

			using RandomGenerator = Framework::Utils::RandomGenerator;
			std::unique_ptr<RandomGenerator> _randomGenerator;
		};
	}
}