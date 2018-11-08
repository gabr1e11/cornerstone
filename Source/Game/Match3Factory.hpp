//
// Match3Factory.hpp
//
// @author Roberto Cano
//

#pragma once

#include "Match3Factory.fwd.hpp"

#include "Framework/Core/Types.hpp"
#include "Framework/Core/GameObject.hpp"

#include "Match3Board.hpp"
#include "Match3Symbol.fwd.hpp"
#include "Match3Cell.fwd.hpp"
#include "Match3ScoreRenderer.fwd.hpp"
#include "Match3Config.hpp"

namespace Match3
{
	namespace Game
	{
		class Factory final : public Framework::Core::GameObject
		{
		public:
			using Board = Match3::Game::Board;
			using Symbol = Match3::Game::Symbol;
			using SymbolType = Match3::Types::Symbol::Type;
			using CellType = Match3::Types::Cell::State;

			using Size = Framework::Types::Size;
			using Point2D = Framework::Types::Point2D;

			Factory(const Config& config);

			void setCurrentLevelNumber(int level);
			int getCurrentLevelNumber() const;

			Framework::Types::GameObject::PtrType Factory::createBackground(const std::string& assetPath, const Point2D& position);
			Types::Cell::PtrType createBoardCell(const Types::Cell::Position& cellPosition);
			Types::Symbol::PtrType createSymbol(SymbolType symbolColor, const Types::Cell::Position& cellPosition);
			Types::Board::PtrType createBoard(IBoardListener* listener);
			Framework::Types::GameObject::PtrType createSoundObject(const std::string& visualDescription);
			Types::ScoreRenderer::PtrType createScoreRenderer(const std::string& prefix, bool isSoundEnabled);

		private:
			const Config::BoardLevel& getCurrentLevel() const;
			const std::string getAssetPathForSymbolType(SymbolType type) const;
			const std::string getAssetPathForShadowType(SymbolType type) const;
			const std::string getAssetPathForCellType(const Config::BoardLevel& level, CellType type) const;
			Point2D getLocalPositionFromCellPosition(const Types::Cell::Position& cellPosition) const;

			Config _config;
			int _currentLevel = 0;
		};
	}
}