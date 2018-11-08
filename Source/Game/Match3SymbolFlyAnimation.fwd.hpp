//
// Match3SymbolFlyAnimation.fwd.hpp
//
// @author Roberto Cano
//

#pragma once

#include <memory>

namespace Match3
{
	namespace Game
	{
		class SymbolFlyAnimation;
	}
	namespace Types
	{
		namespace SymbolFlyAnimation
		{
			using PtrType = std::shared_ptr<Match3::Game::SymbolFlyAnimation>;
		};
	}
}