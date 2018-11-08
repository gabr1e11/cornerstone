//
// Match3Board.cpp
//
// @author Roberto Cano
//

#pragma once

#include "Match3Board.hpp"

#include "Framework/Components/SpriteRenderer.hpp"
#include "Framework/Components/BounceUpDownAnimation.hpp"
#include "Framework/Core/Engine.hpp"
#include "Framework/Utils/EnumIterator.hpp"

#include "Match3Utils.hpp"
#include "Match3Factory.hpp"
#include "Match3MainScene.hpp"
#include "Match3Config.hpp"
#include "Match3Symbol.hpp"
#include "Match3Cell.hpp"
#include "Match3MouseHandling.hpp"

using namespace Match3;
using namespace Match3::Game;
using namespace Match3::Types;
using namespace Framework::Core;
using namespace Framework::Types;

namespace BoardConstants
{
	const Direction LeftDirection = Direction(-1, 0);
	const Direction RightDirection = Direction(1, 0);
	const Direction UpDirection = Direction(0, -1);
	const Direction DownDirection = Direction(0, 1);

	const float WaitTimeForSymbolAnimation = 0.25f;
	const float DropAnimationSpeed = 200.0f;
}

#pragma region - Board
Game::Board::Board(const Size& size, IBoardListener* listener)
	: _symbolsData(size)
	, _cellsData(size)
	, _listener(listener)
{
	// Empty on purpose
}

const Size& Game::Board::getCellsSize() const
{
	return _symbolsData.getSize();
}

const Game::Config& Game::Board::getConfig() const
{
	return getOwner<Game::MainScene>().getConfig();
}

void Game::Board::init()
{
	initConstants();
	setupMouseHandling();
}

void Game::Board::start()
{
	_factory = CreateGameObject<Game::Factory>(getConfig());
	_factory->setCurrentLevelNumber(0);

	_randomGenerator = std::make_unique<Framework::Utils::RandomGenerator>(getSymbolsWeights());

	generateBoard();
	findAndRemoveAllMatches();
}

void Game::Board::update(float dt)
{
	switch (_state)
	{
		case State::WaitingUserInteraction:
		{
			// TODO: Improve this when we have a proper state machine
			setUserInteractionEnabled(true);
			_isUserMatch = true;
			break;
		}
		case State::Delay:
		{
			setUserInteractionEnabled(false);

			_delayTime += dt;
			if (_delayTime >= 5.0f)
			{
				_delayTime = 0.0f;
				_state = State::CheckMatches;
			}
			break;
		}
		case State::CheckMatches:
		{
			setUserInteractionEnabled(false);

			if (_lastMatches.empty())
			{
				_state = State::WaitingUserInteraction;
			}
			else
			{
				_state = State::MatchedSymbolsAnimation;

				_waitTimeForSymbolAnimation = BoardConstants::WaitTimeForSymbolAnimation;

				notifyMatches(_lastMatches);
				removeMatches(_lastMatches);
			}
			break;
		}
		case State::MatchedSymbolsAnimation:
		{
			setUserInteractionEnabled(false);

			updateMatchedSymbols(dt);
			break;
		}
		case State::DroppingSymbols:
		{
			setUserInteractionEnabled(false);
			break;
		}
		case State::PopulateBoard:
		{
			setUserInteractionEnabled(false);

			generateEmptyPositions();
			calculateExistingMatches();

			_isUserMatch = false;

			_state = State::CheckMatches;
			break;
		}
	}
}

#pragma region - Setup
void Game::Board::setupMouseHandling()
{
	Types::MouseHandling::PtrType mouseHandler = CreateComponent<MouseHandling>();
	addComponent(std::move(mouseHandler));
}

void Game::Board::initConstants()
{
	const Size& boardSize = _symbolsData.getSize();

	for (int row = 0; row < boardSize.height; ++row)
	{
		for (int column = 0; column < boardSize.width; ++column)
		{
			Types::Cell::Position cellPosition = Types::Cell::Position(row, column);
			AllBoardPositions.insert(cellPosition);
		}
	}
}

#pragma region - Board generation
void Game::Board::generateBoard()
{
	generateCells(AllBoardPositions);
	generateSymbols(AllBoardPositions);
}

std::vector<float> Game::Board::getSymbolsWeights() const
{
	std::vector<float> weights;

	for (EnumBEIterator<Symbol::Type> iter; iter; iter++)
	{
		Symbol::Type symbolType = *iter;
		const auto& symbolDefinitionIter = getConfig().symbolDefinitions.find(symbolType);
		const Config::SymbolDefinition& symbolDefinition = symbolDefinitionIter->second;

		weights.push_back(symbolDefinition.dropRate);
	}

	return weights;
}

void  Game::Board::generateCells(const CellPositions& specificPositions)
{
	for (const auto& cellPosition : specificPositions)
	{
		Types::Cell::PtrType cell = _factory->createBoardCell(cellPosition);
		addCell(std::move(cell), cellPosition);
	}
}

void Game::Board::addCell(Types::Cell::PtrType cell, const CellPosition& position)
{
	addGameObject(cell);
	_cellsData.at(position) = std::move(cell);
}

void Game::Board::generateSymbols(const CellPositions& specificPositions)
{
	for (const auto& cellPosition : specificPositions)
	{
		assert(_symbolsData.at(cellPosition) == nullptr);

		int randomValue = _randomGenerator->getValue();

		Symbol::Type pieceColor = static_cast<Symbol::Type>(randomValue);

		Types::Symbol::PtrType symbol = _factory->createSymbol(pieceColor, cellPosition);
		addSymbol(std::move(symbol), cellPosition);
	}
}

void Game::Board::addSymbol(Types::Symbol::PtrType symbol, const CellPosition& position)
{
	addGameObject(symbol);

	// Important to do this here after adding the gameobject so the coordinates are
	// adjusted with the parent (the board)
	symbol->setCellPosition(position);
	_symbolsData.at(position) = std::move(symbol);
}

void Game::Board::generateEmptyPositions()
{
	CellPositions emptyPositions = getEmptyCellPositions();
	assert(!emptyPositions.empty());
	generateSymbols(emptyPositions);
}

#pragma region - User interaction
void Game::Board::onUserSelectedSymbol(Types::Symbol::PtrType selectedSymbol, bool isDragDrop)
{
	setSymbolSelected(selectedSymbol, isDragDrop);
}

void Game::Board::onUserPerformedMove(Types::Symbol::PtrType selectedSymbol, const Point2D& toPosition)
{
	const Types::Cell::Position cellPosition = getCellPositionFromWorldPosition(toPosition);

	selectedSymbol->setState(Symbol::State::Idle);

	stopPossibleMovesAnimation(selectedSymbol->getCellPosition());
	setAllCellsState(Cell::State::Normal);

	resolvePossibleMatches(selectedSymbol, cellPosition);
}

void Game::Board::setSymbolSelected(Types::Symbol::PtrType selectedSymbol, bool isDragDrop)
{
	using namespace BoardConstants;

	_state = State::SymbolSelected;

	setAllCellsState(Cell::State::Disabled);

	// Now set the selected cell and its surrounding cells active
	CellPosition cellPosition = selectedSymbol->getCellPosition();

	setCellState(cellPosition, Cell::State::Active);

	setCellState(cellPosition + LeftDirection, Cell::State::Active);
	setCellState(cellPosition + RightDirection, Cell::State::Active);
	setCellState(cellPosition + UpDirection, Cell::State::Active);
	setCellState(cellPosition + DownDirection, Cell::State::Active);

	startPossibleMovesAnimation(cellPosition);

	if (!isDragDrop)
	{
		selectedSymbol->setState(Symbol::State::Selected);
	}
}

void Game::Board::setAllCellsState(Types::Cell::State state)
{
	iterateBoardCells([this, state](const CellPosition& cellPosition)
	{
		setCellState(cellPosition, state);
	});
}

void Game::Board::setCellState(const CellPosition& cellPosition, Cell::State state)
{
	if (!isPositionInsideBoard(cellPosition))
	{
		return;
	}
	Types::Cell::PtrType cell = _cellsData.at(cellPosition);
	cell->setState(state);
}

void Game::Board::startPossibleMovesAnimation(const CellPosition& centerPosition)
{
	using namespace BoardConstants;

	startSymbolPossibleMoveAnimation(centerPosition + UpDirection);
	startSymbolPossibleMoveAnimation(centerPosition + DownDirection);
	startSymbolPossibleMoveAnimation(centerPosition + LeftDirection);
	startSymbolPossibleMoveAnimation(centerPosition + RightDirection);
}

void Game::Board::stopPossibleMovesAnimation(const CellPosition& centerPosition)
{
	using namespace BoardConstants;

	stopSymbolPossibleMoveAnimation(centerPosition + UpDirection);
	stopSymbolPossibleMoveAnimation(centerPosition + DownDirection);
	stopSymbolPossibleMoveAnimation(centerPosition + LeftDirection);
	stopSymbolPossibleMoveAnimation(centerPosition + RightDirection);
}

void Game::Board::startSymbolPossibleMoveAnimation(const CellPosition& cellPosition)
{
	if (!isPositionInsideBoard(cellPosition))
	{
		return;
	}
	Types::Symbol::PtrType symbol = _symbolsData.at(cellPosition);
	symbol->setState(Symbol::State::PossibleMove);
}

void Game::Board::stopSymbolPossibleMoveAnimation(const CellPosition& cellPosition)
{
	if (!isPositionInsideBoard(cellPosition))
	{
		return;
	}
	Types::Symbol::PtrType symbol = _symbolsData.at(cellPosition);
	symbol->setState(Symbol::State::Idle);
}

#pragma region - Matches handling
void Game::Board::findAndRemoveAllMatches()
{
	Matches matches = findAllMatches();
	while (!matches.empty())
	{
		removeMatches(matches);
		generateSymbols(matches);
		matches = findAllMatches();
	};
}

void Game::Board::calculateExistingMatches()
{
	_lastMatches = findAllMatches();
}

Game::Board::Matches Game::Board::calculatePossibleMatches(Types::Symbol::PtrType selectedSymbol, Types::Symbol::PtrType toSymbol)
{
	Matches matches;

	matches.merge(findMatchesForPosition(selectedSymbol->getCellPosition()));
	matches.merge(findMatchesForPosition(toSymbol->getCellPosition()));

	return matches;
}

void Game::Board::resolvePossibleMatches(Types::Symbol::PtrType selectedSymbol, const CellPosition& toPosition)
{
	if (!isPositionInsideBoard(toPosition) ||
		!isValidMove(selectedSymbol, toPosition))
	{
		// Valid case, user clicked outside the board
		_state = State::WaitingUserInteraction;

		swapSymbols(*selectedSymbol, *selectedSymbol);
		return;
	}

	Types::Symbol::PtrType toSymbol = _symbolsData.at(toPosition);
	assert(toSymbol);

	swapSymbols(*selectedSymbol, *toSymbol);

	_lastMatches = calculatePossibleMatches(selectedSymbol, toSymbol);
	if (_lastMatches.empty())
	{
		swapSymbols(*selectedSymbol, *toSymbol);
	}

	_state = State::CheckMatches;
}

bool Game::Board::isValidMove(Types::Symbol::PtrType selectedSymbol, const CellPosition& toPosition) const
{
	using namespace BoardConstants;

	const CellPosition& cellPosition = selectedSymbol->getCellPosition();

	return (toPosition == cellPosition + UpDirection ||
		toPosition == cellPosition + DownDirection ||
		toPosition == cellPosition + LeftDirection ||
		toPosition == cellPosition + RightDirection);
}

void Game::Board::notifyMatches(const Matches& matches)
{
	IBoardListener::MatchedSymbols symbols;
	std::transform(matches.rbegin(), matches.rend(), std::back_inserter(symbols), [this](const CellPosition& cellPosition) -> Types::Symbol::PtrType
	{
		return _symbolsData.at(cellPosition);
	});
	_listener->onSymbolsMatched(_isUserMatch, symbols);
}

void Game::Board::removeMatches(const Matches& matches)
{
	for (const auto& match : matches)
	{
		removeSymbolAtCellPosition(match);
	}
}

void Game::Board::removeSymbolAtCellPosition(const CellPosition& cellPosition)
{
	Types::Symbol::PtrType symbol = _symbolsData.at(cellPosition);
	_symbolsData.at(cellPosition).reset();
	removeGameObject(symbol->getInstanceId());
}

void Game::Board::swapSymbols(Symbol& symbolA, Symbol& symbolB)
{
	const CellPosition originalCellPosition = symbolA.getCellPosition();
	const CellPosition newCellPosition = symbolB.getCellPosition();

	if (symbolA.getInstanceId() == symbolB.getInstanceId())
	{
		symbolA.setCellPosition(newCellPosition);
		return;
	}

	symbolA.setCellPosition(newCellPosition);
	symbolB.setCellPosition(originalCellPosition);

	auto tmp = _symbolsData.at(originalCellPosition);
	_symbolsData.at(originalCellPosition) = _symbolsData.at(newCellPosition);
	_symbolsData.at(newCellPosition) = tmp;
}

Game::Board::Matches Game::Board::findAllMatches()
{
	Matches retValue;

	iterateBoardCells([this, &retValue](const CellPosition& cellPosition)
	{
		retValue.merge(findMatchesForPosition(cellPosition));
	});
	return retValue;
}

Game::Board::Matches Game::Board::findMatchesForPosition(const CellPosition& position)
{
	Matches retValue;

	retValue.merge(findHorizontalMatches(position));
	retValue.merge(findVerticalMatches(position));

	return retValue;
}

Game::Board::Matches Game::Board::findHorizontalMatches(const CellPosition& position)
{
	using namespace BoardConstants;

	Matches retValue;

	retValue.merge(findMatchesForDirection(position, LeftDirection));
	retValue.merge(findMatchesForDirection(position, RightDirection));

	if (retValue.size() < Match3::Game::Globals::MinNumberMatches)
	{
		retValue.clear();
	}

	return retValue;
}

Game::Board::Matches Game::Board::findVerticalMatches(const CellPosition& position)
{
	using namespace BoardConstants;

	Matches retValue;

	retValue.merge(findMatchesForDirection(position, UpDirection));
	retValue.merge(findMatchesForDirection(position, DownDirection));

	if (retValue.size() < Match3::Game::Globals::MinNumberMatches)
	{
		retValue.clear();
	}

	return retValue;
}

Game::Board::Matches Game::Board::findMatchesForDirection(const CellPosition& originalPos, const Direction& direction)
{
	Matches retValue;

	auto symbolPtr = _symbolsData.at(originalPos);

	CellPosition currentPos = originalPos + direction;
	while (currentPos.x >= 0 && currentPos.x < _symbolsData.getSize().width &&
		currentPos.y >= 0 && currentPos.y < _symbolsData.getSize().height)
	{
		auto otherSymbolPtr = _symbolsData.at(currentPos);

		if (otherSymbolPtr && symbolPtr->getType() != otherSymbolPtr->getType())
		{
			break;
		}

		if (retValue.empty())
		{
			retValue.insert(originalPos);
		}
		retValue.insert(currentPos);

		currentPos += direction;
	}
	return retValue;
}


#pragma region - Animations
void Game::Board::updateMatchedSymbols(float dt)
{
	_waitTimeForSymbolAnimation -= dt;
	if (_waitTimeForSymbolAnimation <= 0)
	{
		if (prepareDroppingSymbols())
		{
			_state = State::DroppingSymbols;
		}
		else
		{
			_state = State::PopulateBoard;
		}
	}
}

bool Game::Board::prepareDroppingSymbols()
{
	using namespace BoardConstants;

	bool isAtLeastOneAnimation = false;

	for (int column = 0; column < _symbolsData.getSize().width; ++column)
	{
		int gap = 0;

		for (int row = _symbolsData.getSize().height - 1; row >= 0; --row)
		{
			CellPosition cellPosition(column, row);

			Types::Symbol::PtrType symbol = _symbolsData.at(cellPosition);
			if (symbol)
			{
				if (gap > 0)
				{
					CellPosition targetPosition = cellPosition + gap * DownDirection;
					addDropAnimation(symbol, targetPosition);

					isAtLeastOneAnimation = true;
				}
			}
			else
			{
				++gap;
			}
		}
	}

	return isAtLeastOneAnimation;
}

void Game::Board::addDropAnimation(Types::Symbol::PtrType symbol, const CellPosition& targetCell)
{
	using BounceUpDownAnimation = Framework::Components::BounceUpDownAnimation;

	const CellPosition startCell = symbol->getCellPosition();

	Point2D startPosition = Point2D(startCell.x * symbol->getSize().x, startCell.y * symbol->getSize().y);
	startPosition += symbol->getAnchorPointLocalPosition();

	Point2D endPosition = Point2D(targetCell.x * symbol->getSize().x, targetCell.y * symbol->getSize().y);
	endPosition += symbol->getAnchorPointLocalPosition();

	BounceUpDownAnimation::Config animationConfig;

	animationConfig.start = startPosition;
	animationConfig.end = endPosition;
	animationConfig.speed = BoardConstants::DropAnimationSpeed;

	Framework::Types::BounceUpDownAnimation::PtrType bounceAnimation = CreateComponent<BounceUpDownAnimation>(animationConfig);

	auto onAnimationFinishedCB = std::bind(&Game::Board::onDropAnimationFinished, this, std::placeholders::_1);
	bounceAnimation->setOnFinishedCallback(onAnimationFinishedCB);
	bounceAnimation->run();

	symbol->addComponent(bounceAnimation);
	symbol->setState(Symbol::State::Dropping);

	_finalSymbolPositions.emplace_back(FinalPosition{ symbol, targetCell });
	_symbolsData.at(startCell).reset();

	++_numFallingSymbols;
}

void Game::Board::onDropAnimationFinished(Framework::Components::BounceUpDownAnimation& bounceAnimation)
{
	// TODO: this crashes, haven't discovered why
	// bounceAnimation.getOwner().removeComponent(bounceAnimation);

	if (--_numFallingSymbols == 0)
	{
		// Reallocate the symbols
		for (auto& finalPosition : _finalSymbolPositions)
		{
			finalPosition.symbol->setCellPosition(finalPosition.cellPosition);
			finalPosition.symbol->setState(Symbol::State::Idle);
			_symbolsData.at(finalPosition.cellPosition) = finalPosition.symbol;
		}
		_finalSymbolPositions.clear();

		_state = State::PopulateBoard;
	}
}

#pragma region - Utils
Game::Board::CellPosition Game::Board::getCellPositionFromWorldPosition(const Point2D& worldPosition) const
{
	const Point2D localPosition = worldPosition - getWorldPosition();

	int column = static_cast<int>((localPosition.x / getConfig().cellSizeInPixels.width));
	int row = static_cast<int>((localPosition.y / getConfig().cellSizeInPixels.height));

	return CellPosition(column, row);
}

Types::Symbol::PtrType Game::Board::getSymbolFromWorldPosition(const Point2D& worldPosition)
{
	CellPosition cellPosition = getCellPositionFromWorldPosition(worldPosition);
	if (cellPosition.x < 0 || cellPosition.x >= getConfig().boardSizeInCells.width ||
		cellPosition.y < 0 || cellPosition.y >= getConfig().boardSizeInCells.height)
	{
		return nullptr;
	}
	return _symbolsData.at(cellPosition);
}

bool Game::Board::isPositionInsideBoard(const CellPosition& cellPosition) const
{
	return (cellPosition.x >= 0 && cellPosition.x < _symbolsData.getSize().width &&
		cellPosition.y >= 0 && cellPosition.y < _symbolsData.getSize().height);
}

void Game::Board::iterateBoardCells(const BoardIteratorFunc& func) const
{
	const Size& boardSize = _symbolsData.getSize();
	for (int row = 0; row < boardSize.height; ++row)
	{
		for (int column = 0; column < boardSize.width; ++column)
		{
			const CellPosition cellPosition(column, row);
			func(cellPosition);
		}
	}
}

void Game::Board::iterateBoardCells(const BoardIteratorFunc& func)
{
	const Size& boardSize = _symbolsData.getSize();
	for (int row = 0; row < boardSize.height; ++row)
	{
		for (int column = 0; column < boardSize.width; ++column)
		{
			const CellPosition cellPosition(column, row);
			func(cellPosition);
		}
	}
}

Game::Board::CellPositions Game::Board::getEmptyCellPositions() const
{
	CellPositions emptyPositions;

	iterateBoardCells([this, &emptyPositions](const CellPosition& cellPosition)
	{
		Types::Symbol::PtrType symbol = _symbolsData.at(cellPosition);
		if (symbol == nullptr)
		{
			emptyPositions.insert(cellPosition);
		}
	});

	return emptyPositions;
}


int Game::Board::getNumberGapsFromCellPosition(const Direction& direction, const CellPosition& fromPosition) const
{
	int gap = 1; // The fromPosition should be empty
	CellPosition position = fromPosition;

	position += direction;
	while (isPositionInsideBoard(position))
	{
		Types::Symbol::PtrType symbol = _symbolsData.at(position);
		if (symbol)
		{
			break;
		}

		++gap;
		position += direction;
	}

	return gap;
}

std::vector<Types::Symbol::PtrType> Game::Board::getAllSymbolsInDirection(const Direction& direction, const CellPosition& fromPosition)
{
	std::vector<Types::Symbol::PtrType> foundSymbols;
	CellPosition position = fromPosition;

	position += direction;
	while (isPositionInsideBoard(position))
	{
		Types::Symbol::PtrType symbol = _symbolsData.at(position);
		if (symbol)
		{
			foundSymbols.push_back(std::move(symbol));
			_symbolsData.at(position) = nullptr;
		}
		position += direction;
	}

	return foundSymbols;
}

Types::Symbol::PtrType Game::Board::getSymbolInDirection(const Direction& direction, const CellPosition& fromPosition)
{
	CellPosition position = fromPosition + direction;
	if (isPositionInsideBoard(position))
	{
		return _symbolsData.at(position);
	}
	else
	{
		return nullptr;
	}
}

void Game::Board::setUserInteractionEnabled(bool enabled)
{
	_isUserInteractionEnabled = enabled;
}

bool Game::Board::isUserInteractionEnabled() const
{
	return _isUserInteractionEnabled;
}