//
// Match3MainScene.cpp
//
// @author Roberto Cano
//

#include "Match3MainScene.hpp"

#include "Match3Factory.hpp"
#include "Match3Symbol.hpp"
#include "Match3Board.hpp"
#include "Match3Config.hpp"
#include "Match3Globals.hpp"
#include "Match3MouseHandling.hpp"
#include "Match3ScoreRenderer.hpp"
#include "Match3SymbolFlyAnimation.hpp"

#include "Framework/Core/Types.hpp"
#include "Framework/Core/Engine.hpp"
#include "Framework/Components/SpriteRenderer.hpp"
#include "Framework/Components/TextRenderer.hpp"
#include "Framework/Components/SoundEmitter.hpp"

using namespace Framework::Core;
using namespace Framework::Types;
using namespace Match3::Game;

namespace MainSceneConstants
{
	const std::string GameTitle = "Match3 Game!";
	const float GameTitleScale = 1.0f;
	const float GameTitlePosFromTop = 40.0f;
	const float GameTitleAnimationStrength = 6.0f;
	const float GameTitlePeriodSecs = 1.0f;
	const float GameTitleCharacterSeparation = 3.0f;
	const float GameTitleCharacterPhaseShiftTime = 0.2f;
	const Color3D GameTitleColor = Color3D(0.0f, 0.0f, 0.0f);

	const std::string ScoreText = "Score: ";
	const float ScorePosFromLeft = 130.0f;
	const float ScorePosFromBottom = 60.0f;

	const std::string MovesLeftText = "Moves: ";
	const float MovesLeftPosFromLeft = 380.0f;
	const float MovesLeftPosFromBottom = 60.0f;

	const Point2D SceneAnchorPoint = Point2D(0.5f, 0.5f);

	const int PointsPerMatchedSymbol = 10;

	const float SymbolAnimationDelayIncrement = 0.05f;
	const Point2D SymbolAnimationTargetOffset = Point2D(40.0f, 0.0f);
}

MainScene::MainScene(const Config& config)
	: Scene(Size2D(config.engineSettings.windowSize))
	, _config(config)
{
	_config.validate();

	setZIndex(Match3::Game::Globals::MainSceneZIndex);
}

void MainScene::start()
{
	_factory = std::move(CreateGameObject<Game::Factory>(_config));

	setupBackground();
	setupBoard();
	setupScoreRenderers();
}

void MainScene::update(float dt)
{
	renderTitle(dt);
}

const Config& MainScene::getConfig() const
{
	return _config;
}

#pragma region - Setup
void MainScene::setupBoard()
{
	setAnchorPoint(MainSceneConstants::SceneAnchorPoint);

	_board = std::move(_factory->createBoard(this));

	Size2D screenSize = getEngine()->getScreenSize();
	Size2D boardSize = _board->getSize();

	Point2D boardPosition = (screenSize - boardSize) / 2.0f;
	_board->setWorldPosition(boardPosition);

	addGameObject(_board);
}

void MainScene::setupScoreRenderers()
{
	using namespace MainSceneConstants;

	_scoreRenderer = _factory->createScoreRenderer(ScoreText, true);
	_scoreRenderer->setLocalPosition(getScoreTextPosition());

	addGameObject(_scoreRenderer);

	_movesCountRenderer = _factory->createScoreRenderer(MovesLeftText, false);
	_movesCountRenderer->setLocalPosition(getMovesTextPosition());

	addGameObject(_movesCountRenderer);
}

void MainScene::setupBackground()
{
	const Point2D& screenMidPoint = getScreenMidPoint();
	const std::string& backgroundAssetName = getConfig().backgroundAsset;

	auto background = _factory->createBackground(backgroundAssetName, screenMidPoint);

	addGameObject(background);
}

#pragma region - HUD
float MainScene::getTitleWidth() const
{
	using namespace MainSceneConstants;

	Framework::Core::Engine& engine = *getEngine();

	float titleWidth = 0;
	for (const char& character : GameTitle)
	{
		std::string textToRender = std::string(1, character);
		float characterWidth = engine.getTextSize(textToRender, GameTitleScale).x;
		titleWidth += characterWidth + GameTitleCharacterSeparation;
	}

	return titleWidth;
}

void MainScene::renderTitle(float dt)
{
	using namespace MainSceneConstants;

	_titleAnimationTime += dt;

	const Size& screenSize = getEngine()->getScreenSize();

	std::vector<float> yOffsets;

	for (size_t numLetter = 0; numLetter < GameTitle.size(); ++numLetter)
	{
		float phaseShiftedTime = _titleAnimationTime + numLetter * GameTitleCharacterPhaseShiftTime;
		yOffsets.push_back(GameTitleAnimationStrength * glm::sin(phaseShiftedTime * 2.0f * glm::pi<float>() / GameTitlePeriodSecs));
	}

	const Point2D& mainTitlePosition = getTitleTextPosition();
	getEngine()->renderText(GameTitle, mainTitlePosition, yOffsets, Match3::Game::Globals::TextRenderingZIndex, GameTitleColor, 0.0f, GameTitleScale);
}

Point2D MainScene::getTitleTextPosition() const
{
	using namespace MainSceneConstants;

	const Size& screenSize = getEngine()->getScreenSize();
	return Point2D(screenSize.width / 2.0f, GameTitlePosFromTop);
}

Point2D MainScene::getScoreTextPosition() const
{
	using namespace MainSceneConstants;

	const Size& screenSize = getEngine()->getScreenSize();
	return Point2D(ScorePosFromLeft, screenSize.height - ScorePosFromBottom);
}

Point2D MainScene::getMovesTextPosition() const
{
	using namespace MainSceneConstants;

	const Size& screenSize = getEngine()->getScreenSize();
	return Point2D(MovesLeftPosFromLeft, screenSize.height - MovesLeftPosFromBottom);
}

#pragma region - Events
void MainScene::onSymbolsMatched(bool isUserTriggered, MatchedSymbols& matchedSymbols)
{
	if (isUserTriggered)
	{
		incrementMoves();
	}

	animateFlyingSymbols(matchedSymbols);
}

void MainScene::onSymbolFlyAnimationFinished(SymbolPtrType symbol)
{
	incrementScore();
	removeGameObject(*symbol);
}

#pragma region - Animations
void MainScene::animateFlyingSymbols(MatchedSymbols& matchedSymbols)
{
	float phase = 0.0f;
	float delay = 0.0f;
	for (auto& symbol : matchedSymbols)
	{
		Types::SymbolFlyAnimation::PtrType flyAnimation = CreateGameObject<SymbolFlyAnimation>(symbol, getScoreTextPosition() + MainSceneConstants::SymbolAnimationTargetOffset, delay);
		addGameObject(std::move(flyAnimation));

		delay += MainSceneConstants::SymbolAnimationDelayIncrement;
	}
}

#pragma region - User state
void MainScene::incrementMoves()
{
	_movesCountRenderer->addScore(1);
}

void MainScene::incrementScore()
{
	_scoreRenderer->addScore(MainSceneConstants::PointsPerMatchedSymbol);
}

#pragma region - Utils
Point2D MainScene::getScreenMidPoint() const
{
	Size2D screenSize = getEngine()->getScreenSize();
	return screenSize / 2.0f;
}