//
// Match3Cell.hpp
//
// @author Roberto Cano
//

#pragma once

#include "Match3Cell.fwd.hpp"

#include "Framework/Core/Types.hpp"
#include "Framework/Core/GameObject.hpp"

namespace Match3
{
	namespace Game
	{
		class Cell final : public Framework::Core::GameObject
		{
		public:
			using Size = Framework::Types::Size;
			using State = Types::Cell::State;
			using StateMap = std::unordered_map<State, std::string>;
			using SpriteRenderer = Framework::Types::SpriteRenderer::PtrType;

			struct Config
			{
				StateMap stateMap;
				Size size;
			};

			Cell(const Config& cellConfig);

			void init() override;

			void setState(State state);
			State getState() const;

		private:
			void addSpriteRenderer();
			const std::string& getAssetPathForCurrentState() const;
			void updateSpriteRendererWithState();

			StateMap _stateMap;
			SpriteRenderer _spriteRenderer;

			State _state = State::Normal;
		};
	}
}
