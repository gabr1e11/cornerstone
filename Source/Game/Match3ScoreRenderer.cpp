//
// Match3ScoreRenderer.cpp
//
// @author Roberto Cano
//

#include "Match3ScoreRenderer.hpp"

#include "Framework/Components/TextRenderer.hpp"
#include "Framework/Components/SoundEmitter.hpp"
#include "Framework/Core/Types.hpp"

#include "Match3Factory.hpp"
#include "Match3MainScene.hpp"

using namespace Framework::Types;

namespace ScoreRendererConstants
{
	const float TextScale = 0.5f;
	const Color3D TextColor = Color3D(0.0f, 0.0f, 0.0f);
}

void Match3::Game::ScoreRenderer::setPrefix(const std::string & prefix)
{
	_prefix = prefix;

	updateTextRenderer();
}

const std::string & Match3::Game::ScoreRenderer::getPrefix() const
{
	return _prefix;
}

void Match3::Game::ScoreRenderer::addScore(int score)
{
	_score += score;

	updateTextRenderer();
	triggerSoundEmitter();
}

void Match3::Game::ScoreRenderer::resetScore()
{
	_score = 0;
}

void Match3::Game::ScoreRenderer::setSoundEnabled(bool enabled)
{
	_isSoundEnabled = enabled;
}

bool Match3::Game::ScoreRenderer::isSoundEnabled() const
{
	return _isSoundEnabled;
}

void Match3::Game::ScoreRenderer::setupRandomGenerator()
{
	_randomGenerator = std::make_unique<Framework::Utils::RandomGenerator>(0, 9);
}

void Match3::Game::ScoreRenderer::setupFactory()
{
	const auto& scene = getOwner<MainScene>();
	const auto& config = scene.getConfig();

	_factory = std::move(CreateGameObject<Game::Factory>(config));
}

void Match3::Game::ScoreRenderer::setupTextRenderer()
{
	using namespace ScoreRendererConstants;

	_textRenderer = CreateComponent<Framework::Components::TextRenderer>();
	_textRenderer->setScale(TextScale);
	_textRenderer->setColor(TextColor);

	addComponent(_textRenderer);
}

void Match3::Game::ScoreRenderer::setupSoundEmitter()
{
	_soundEmitter = CreateComponent<Framework::Components::SoundEmitter>();

	_soundEmitter->setVisualDescription("Clink!");

	addComponent(_soundEmitter);
}

void Match3::Game::ScoreRenderer::updateTextRenderer()
{
	if (_textRenderer)
	{
		std::string fullText = _prefix + std::to_string(_score);

		_textRenderer->setText(fullText);
	}
}

void Match3::Game::ScoreRenderer::triggerSoundEmitter()
{
	if (_isSoundEnabled)
	{
		int randomIndex = _randomGenerator->getValue();

		float randomAngle = -glm::pi<float>() / 8.0f + randomIndex * glm::pi<float>() / 32.0f;
		_soundEmitter->setRotation(randomAngle);

		_soundEmitter->play();
	}
}

void Match3::Game::ScoreRenderer::start()
{
	setupRandomGenerator();
	setupFactory();
	setupTextRenderer();
	setupSoundEmitter();

	updateTextRenderer();
}
