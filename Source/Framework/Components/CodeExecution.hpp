//
// CodeExecution.hpp
//
// @author Roberto Cano
//

#pragma once

#include <functional>

#include "Framework/Core/GameComponent.hpp"

namespace Framework
{
    namespace Components
    {
        class CodeExecution : public Core::GameComponent
        {
        public:
            using UpdateFunction = std::function<void(float dt)>;

            CodeExecution(UpdateFunction& updateFunction);
            ~CodeExecution() = default;

            void init() override;
            void start() override;
            void update(float dt) override;

        private:
            UpdateFunction _updateFunction;
        };
    }
}
