//
// Match3Factory.fwd.hpp
//
// @author Roberto Cano
//

#pragma once

#include <memory>

namespace Match3
{
    namespace Game
    {
        class Factory;
    }
    namespace Types
    {
        namespace Factory
        {
            using PtrType = std::shared_ptr<Match3::Game::Factory>;
        };
    }
}