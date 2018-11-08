//
// Match3Cell.fwd.hpp
//
// @author Roberto Cano
//

#pragma once

#include <memory>
#include <glm/glm.hpp>

namespace Match3
{
	namespace Game
	{
		class Cell;
	}
	namespace Types
	{
		namespace Cell
		{
			using PtrType = std::shared_ptr<Match3::Game::Cell>;
			using Position = glm::ivec2;

			enum class State : int
			{
				Normal,
				Active,
				Disabled
			};
		};
	}
}
