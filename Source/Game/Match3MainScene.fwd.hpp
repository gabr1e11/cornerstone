//
// Match3MainScene.fwd.hpp
//
// @author Roberto Cano
//

#pragma once

#include <memory>

namespace Match3
{
    namespace Game
    {
        class MainScene;
    }
    namespace Types
    {
        namespace MainScene
        {
            using PtrType = std::shared_ptr<Match3::Game::MainScene>;
        };
    }
}

