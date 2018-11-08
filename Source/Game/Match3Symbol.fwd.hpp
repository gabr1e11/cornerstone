//
// Match3Symbol.fwd.hpp
//
// @author Roberto Cano
//

#pragma once

#include <memory>

namespace Match3
{
	namespace Game
	{
		class Symbol;
	}
	namespace Types
	{
		namespace Symbol
		{
			using PtrType = std::shared_ptr<Match3::Game::Symbol>;

			enum class Type : int
			{
				Candy,
				_begin = Candy, // Little trick so the right value appears in the debugger
				Cookie,
				Donut,
				Muffin,
				Pie,
				_end
			};
		};
	}
}