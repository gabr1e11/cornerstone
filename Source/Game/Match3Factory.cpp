//
// Match3Factory.cpp
//
// @author Roberto Cano
//

#include <iterator>

#include "Match3Factory.hpp"

#include "Framework/Components/SpriteRenderer.hpp"
#include "Framework/Components/MouseHandler.hpp"
#include "Framework/Components/SoundEmitter.hpp"

#include "Match3Utils.hpp"
#include "Match3Board.hpp"
#include "Match3Symbol.hpp"
#include "Match3Cell.hpp"
#include "Match3ScoreRenderer.hpp"

using namespace Match3;
using namespace Match3::Game;
using namespace Framework::Types;

namespace FactoryConstants
{
	const Point2D SymbolAnchorPoint(0.5f, 0.5f);
	const Point2D CellAnchorPoint(0.0f, 0.0f);
	const Point2D SymbolAvatarAnchorPoint(0.4f, 0.6f);
	const Point2D SymbolShadowAnchorPoint(0.58f, 0.42f);
	const Point2D BackgroundAnchorPoint = Point2D(0.5f, 0.5f);
	const Color3D TextDefaultColor = Color3D(0.0f, 0.0f, 0.0f);
}

Factory::Factory(const Config& config)
	: _config(config)
{
	// Empty on purpose
}

void Factory::setCurrentLevelNumber(int level)
{
	_currentLevel = level;
}

int Factory::getCurrentLevelNumber() const
{
	return _currentLevel;
}

const Config::BoardLevel& Factory::getCurrentLevel() const
{
	int levelNumber = getCurrentLevelNumber();
	assert(static_cast<size_t>(levelNumber) < _config.levels.size());
	return _config.levels.at(levelNumber);
}

Framework::Types::GameObject::PtrType Factory::createBackground(const std::string& assetPath, const Point2D& position)
{
	using namespace Framework::Components;
	using namespace Framework::Types;
	using namespace Match3::Game::Globals;
	using namespace FactoryConstants;

	Framework::Types::GameObject::PtrType gameObject = CreateGameObject<Framework::Core::GameObject>();
	gameObject->setAnchorPoint(Point2D(0.5f, 0.5f));
	gameObject->setWorldPosition(position);
	gameObject->setZIndex(BackgroundZIndex);

	Framework::Types::SpriteRenderer::PtrType spriteRenderer = CreateComponent<Framework::Components::SpriteRenderer>(assetPath);
	spriteRenderer->setAnchorPoint(BackgroundAnchorPoint);

	gameObject->addComponent(std::move(spriteRenderer));

	return gameObject;
}

Types::Symbol::PtrType Factory::createSymbol(SymbolType symbolColor, const Types::Cell::Position& cellPosition)
{
	using namespace Framework::Components;
	using namespace Framework::Types;
	using namespace Match3::Game::Globals;
	using namespace FactoryConstants;

	const Size& symbolSize = _config.cellSizeInPixels;
	std::shared_ptr<Symbol> symbol = CreateGameObject<Symbol>(symbolColor, symbolSize);
	assert(symbol);
	symbol->setState(Symbol::State::Appearing);

	// Shadow sprite renderer
	const std::string& shadowAssetPath = getAssetPathForShadowType(symbolColor);
	Framework::Types::SpriteRenderer::PtrType shadowSpriteRenderer = CreateComponent<Framework::Components::SpriteRenderer>(shadowAssetPath);
	shadowSpriteRenderer->setAnchorPoint(FactoryConstants::SymbolAnchorPoint);
	shadowSpriteRenderer->setVisible(false);
	shadowSpriteRenderer->setAnchorPoint(SymbolShadowAnchorPoint);
	shadowSpriteRenderer->setZIndexOffset(0);
	shadowSpriteRenderer->setName(ShadowSpriteRendererName);

	symbol->addComponent(std::move(shadowSpriteRenderer));

	// Symbol sprite renderer
	const std::string& symbolAssetPath = getAssetPathForSymbolType(symbolColor);
	Framework::Types::SpriteRenderer::PtrType symbolSpriteRenderer = CreateComponent<Framework::Components::SpriteRenderer>(symbolAssetPath);
	symbolSpriteRenderer->setAnchorPoint(FactoryConstants::SymbolAnchorPoint);
	symbolSpriteRenderer->setVisible(false);
	symbolSpriteRenderer->setAnchorPoint(SymbolAvatarAnchorPoint);
	symbolSpriteRenderer->setZIndexOffset(1);
	symbolSpriteRenderer->setName(SymbolSpriteRendererName);

	symbol->addComponent(std::move(symbolSpriteRenderer));

	return std::move(symbol);
}

Types::Cell::PtrType Factory::createBoardCell(const Types::Cell::Position& cellPosition)
{
	using namespace Framework::Components;
	using namespace Framework::Types;
	using namespace Match3::Game::Globals;

	const Config::BoardLevel& currentLevel = getCurrentLevel();
	const Size& cellSize = _config.cellSizeInPixels;

	Cell::Config cellConfig;

	cellConfig.size = cellSize;

	std::transform(currentLevel.boardCellDefinitions.begin(), currentLevel.boardCellDefinitions.end(),
		std::inserter(cellConfig.stateMap, cellConfig.stateMap.begin()),
		[](const typename Config::BoardCellDefinitions::value_type cellDefPair) -> typename Cell::StateMap::value_type
	{
		typename Cell::StateMap::value_type stateMapPair(cellDefPair.second.type, cellDefPair.second.cellAssetName);
		return stateMapPair;
	});

	Types::Cell::PtrType cell = CreateGameObject<Cell>(cellConfig);
	assert(cell);
	cell->setZIndex(CellZIndex);

	const Point2D position = getLocalPositionFromCellPosition(cellPosition);
	cell->setAnchorPoint(FactoryConstants::CellAnchorPoint);
	cell->setLocalPosition(position);

	return std::move(cell);
}

Types::Board::PtrType Factory::createBoard(IBoardListener* listener)
{
	auto board = CreateGameObject<Board>(_config.boardSizeInCells, listener);

	Size boardSizeInPixels = Size(_config.boardSizeInCells.width * _config.cellSizeInPixels.width,
		_config.boardSizeInCells.height * _config.cellSizeInPixels.height);

	board->setSize(boardSizeInPixels);

	return board;
}

Framework::Types::GameObject::PtrType Factory::createSoundObject(const std::string& visualDescription)
{
	auto gameObject = CreateGameObject<GameObject>();

	// Sound emitter
	auto soundEmitter = CreateComponent<Framework::Components::SoundEmitter>();
	soundEmitter->setVisualDescription(visualDescription);

	gameObject->addComponent(std::move(soundEmitter));

	return gameObject;
}

Types::ScoreRenderer::PtrType Factory::createScoreRenderer(const std::string& prefix, bool isSoundEnabled)
{
	auto scoreRenderer = CreateGameObject<ScoreRenderer>();
	scoreRenderer->setPrefix(prefix);
	scoreRenderer->setSoundEnabled(isSoundEnabled);
	return scoreRenderer;
}

const std::string Factory::getAssetPathForSymbolType(SymbolType type) const
{
	assert(_config.symbolDefinitions.find(type) != _config.symbolDefinitions.end());
	return _config.symbolDefinitions.at(type).avatarAssetName;
}

const std::string Factory::getAssetPathForShadowType(SymbolType type) const
{
	assert(_config.symbolDefinitions.find(type) != _config.symbolDefinitions.end());
	return _config.symbolDefinitions.at(type).shadowAssetName;
}

const std::string Factory::getAssetPathForCellType(const Config::BoardLevel& level, CellType type) const
{
	assert(level.boardCellDefinitions.find(type) != level.boardCellDefinitions.end());
	return level.boardCellDefinitions.at(type).cellAssetName;
}

Point2D Factory::getLocalPositionFromCellPosition(const Types::Cell::Position& cellPosition) const
{
	const Size& boardSizeInCells = _config.boardSizeInCells;
	const Size& cellSizeInPixels = _config.cellSizeInPixels;

	return Point2D(cellPosition.x * cellSizeInPixels.width,
		cellPosition.y * cellSizeInPixels.height);
}
