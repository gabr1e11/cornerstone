//
// Match3Board.hpp
//
// @author Roberto Cano
//

#pragma once

#include "Match3Board.fwd.hpp"

#include "Framework/Core/Types.hpp"
#include "Framework/Core/GameObject.hpp"
#include "Framework/Utils/Utils.hpp"

#include "Match3Symbol.fwd.hpp"
#include "Match3Cell.fwd.hpp"
#include "Match3Factory.fwd.hpp"
#include "Match3Globals.hpp"
#include "Match3Utils.hpp"

namespace Match3
{
	namespace Game
	{
#pragma region - BoardListener
		class IBoardListener
		{
		public:
			using MatchedSymbols = std::vector<Types::Symbol::PtrType>;

			virtual void onSymbolsMatched(bool isUserTriggered, MatchedSymbols& matchedSymbols) = 0;
		};

#pragma region - Board
		struct Config;
		class Board final : public Framework::Core::GameObject
		{
		public:
			using Size = Framework::Types::Size;
			using Point2D = Framework::Types::Point2D;
			using Symbol = Match3::Game::Symbol;
			using GameObject = Framework::Core::GameObject;
			using CellPosition = Types::Cell::Position;
			using Direction = Framework::Types::Direction;
			using CellPositions = Types::Board::CellPositions;
			using Matches = Types::Board::Matches;
			using BoardData = Match3::Utils::Matrix2x2<Types::Symbol::PtrType>;
			using CellsData = Match3::Utils::Matrix2x2<Types::Cell::PtrType>;

			Board(const Size& size, IBoardListener* listener);

			void init() override;
			void start() override;
			void update(float dt) override;

#pragma region - Utils
			const Size& getCellsSize() const;

			CellPosition getCellPositionFromWorldPosition(const Point2D& worldPosition) const;
			Types::Symbol::PtrType getSymbolFromWorldPosition(const Point2D& worldPosition);
			bool isPositionInsideBoard(const CellPosition& cellPosition) const;

			bool isUserInteractionEnabled() const;

		protected:
#pragma region - User interaction
			friend class MouseHandling;
			void onUserSelectedSymbol(Types::Symbol::PtrType selectedSymbol, bool isDragDrop);
			void onUserPerformedMove(Types::Symbol::PtrType selectedSymbol, const Point2D& toPosition);

			void setAllCellsState(Types::Cell::State state);
			void setSymbolSelected(Types::Symbol::PtrType selectedSymbol, bool isDragDrop);
			void setCellState(const CellPosition& cellPosition, Types::Cell::State state);
			void startPossibleMovesAnimation(const CellPosition& centerPosition);
			void stopPossibleMovesAnimation(const CellPosition& centerPosition);
			void startSymbolPossibleMoveAnimation(const CellPosition& cellPosition);
			void stopSymbolPossibleMoveAnimation(const CellPosition& cellPosition);

		private:
#pragma region - Setup
			void initConstants();
			void setupMouseHandling();

			const Game::Config& getConfig() const;

#pragma region - Board generation
			std::vector<float> getSymbolsWeights() const;
			void generateBoard();
			void generateCells(const CellPositions& specificPositions);
			void addCell(Types::Cell::PtrType cell, const CellPosition& position);
			void generateSymbols(const CellPositions& specificPositions);
			void addSymbol(Types::Symbol::PtrType symbol, const CellPosition& position);
			void generateEmptyPositions();

#pragma region - Match finding
			void calculateExistingMatches();
			void findAndRemoveAllMatches();
			Matches calculatePossibleMatches(Types::Symbol::PtrType selectedSymbol, Types::Symbol::PtrType toSymbol);
			void resolvePossibleMatches(Types::Symbol::PtrType selectedSymbol, const CellPosition& cellPosition);
			void notifyMatches(const Matches& matches);

			void removeMatches(const Matches& matches);
			void removeSymbolAtCellPosition(const CellPosition& cellPosition);
			void swapSymbols(Symbol& _selectedSymbol, Symbol& symbol);

			Matches findAllMatches();
			Matches findMatchesForPosition(const CellPosition& position);
			Matches findHorizontalMatches(const CellPosition& position);
			Matches findVerticalMatches(const CellPosition& position);
			Matches findMatchesForDirection(const CellPosition& position, const Direction& direction);

#pragma region - Animations
			void updateMatchedSymbols(float dt);
			bool prepareDroppingSymbols();
			void addDropAnimation(Types::Symbol::PtrType symbol, const CellPosition& targetCell);

			void onDropAnimationFinished(Framework::Components::BounceUpDownAnimation& bounceAnimation);

#pragma region - Internal utils
			using BoardIteratorFunc = std::function<void(const CellPosition& cell)>;
			void iterateBoardCells(const BoardIteratorFunc& func);
			void iterateBoardCells(const BoardIteratorFunc& func) const;
			bool isValidMove(Types::Symbol::PtrType selectedSymbol, const CellPosition& toPosition) const;
			std::vector<Types::Symbol::PtrType> getAllSymbolsInDirection(const Direction& direction, const CellPosition& fromPosition);
			Types::Symbol::PtrType getSymbolInDirection(const Direction& direction, const CellPosition& fromPosition);
			int getNumberGapsFromCellPosition(const Direction& direction, const CellPosition& fromPosition) const;
			CellPositions getEmptyCellPositions() const;
			void setUserInteractionEnabled(bool enabled);

#pragma region - Data
			CellPositions AllBoardPositions;

			enum class State
			{
				WaitingUserInteraction,
				SymbolSelected,
				Delay,
				CheckMatches,
				MatchedSymbolsAnimation,
				DroppingSymbols,
				PopulateBoard
			};

			bool _isUserInteractionEnabled = true;

			float _delayTime = 0.0f;
			State _state = State::WaitingUserInteraction;

			Types::Factory::PtrType _factory;

			BoardData _symbolsData;
			CellsData _cellsData;

			std::unique_ptr<Framework::Utils::RandomGenerator> _randomGenerator;
			IBoardListener* _listener = nullptr;

#pragma region - Board regeneration data
			struct FinalPosition
			{
				Types::Symbol::PtrType symbol;
				CellPosition cellPosition;
			};
			std::vector<FinalPosition> _finalSymbolPositions;

#pragma region - Animation data
			struct DropAnimationItem
			{
				Types::Symbol::PtrType symbol;
				CellPosition targetCell;
				Point2D targetPosition;
			};

			Matches _lastMatches;
			std::vector<DropAnimationItem> _droppingSymbolsAnimation;
			float _waitTimeForSymbolAnimation = 0.0f;
			int _numFallingSymbols = 0;
			bool _isUserMatch = true;
		};
	}
}