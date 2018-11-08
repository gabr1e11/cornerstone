//
// Match3MouseHandling.fwd.hpp
//
// @author Roberto Cano
//

#pragma once

#include <memory>

namespace Match3
{
	namespace Game
	{
		class MouseHandling;
	}
	namespace Types
	{
		namespace MouseHandling
		{
			using PtrType = std::shared_ptr<Match3::Game::MouseHandling>;
		};
	}
}