//
// Match3ScoreRenderer.fwd.hpp
//
// @author Roberto Cano
//

#pragma once

#include <memory>

namespace Match3
{
	namespace Game
	{
		class ScoreRenderer;
	}
	namespace Types
	{
		namespace ScoreRenderer
		{
			using PtrType = std::shared_ptr<Match3::Game::ScoreRenderer>;
		};
	}
}