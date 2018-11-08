//
// Match3Board.fwd.hpp
//
// @author Roberto Cano
//

#pragma once

#include <memory>
#include <set>
#include <glm\glm.hpp>

#include "Match3Globals.hpp"
#include "Match3Utils.hpp"

namespace Match3
{
	namespace Game
	{
		class Board;
	}
	namespace Types
	{
		namespace Board
		{
			using PtrType = std::shared_ptr<Match3::Game::Board>;

			using Position = glm::ivec2;

			struct CompareCellPosition {
				bool operator()(const Position& a, const Position& b) {
					return (a.x * Game::Globals::MaxBoardSize + a.y) <
						(b.x * Game::Globals::MaxBoardSize + b.y);
				}
			};
			using CellPositions = std::set<Position, CompareCellPosition>;
			using Matches = CellPositions;
		};
	}
}