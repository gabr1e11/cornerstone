//
// Match3MainScene.hpp
//
// @author Roberto Cano
//

#pragma once

#include "Match3MainScene.fwd.hpp"

#include "Framework/Core/Scene.hpp"
#include "Framework/Utils/Utils.hpp"

#include "Match3Board.hpp"
#include "Match3Symbol.fwd.hpp"
#include "Match3Factory.fwd.hpp"
#include "Match3ScoreRenderer.fwd.hpp"
#include "Match3Config.hpp"

namespace Match3
{
	namespace Game
	{
		class MainScene final : public Framework::Core::Scene, public IBoardListener
		{
		public:
			MainScene(const Config& config);
			~MainScene() = default;

			void start() override;
			void update(float dt) override;

			const Config& getConfig() const;

		protected:
			using SymbolPtrType = Types::Symbol::PtrType;

#pragma region - Events
			friend class Board;
			friend class SymbolFlyAnimation;
			void onSymbolsMatched(bool isUserTriggered, MatchedSymbols& matchedSymbols) override;
			void onSymbolFlyAnimationFinished(SymbolPtrType symbol);

		private:
#pragma region - Setup
			void setupBoard();
			void setupScoreRenderers();
			void setupBackground();

#pragma region - HUD
			float getTitleWidth() const;
			void renderTitle(float dt);
			Point2D getTitleTextPosition() const;
			Point2D getScoreTextPosition() const;
			Point2D getMovesTextPosition() const;

#pragma region - Animations
			void animateFlyingSymbols(MatchedSymbols& matchedSymbols);

#pragma region - User state
			void incrementMoves();
			void incrementScore();

#pragma region - Utils
			Point2D getScreenMidPoint() const;

#pragma region - Data
			Config _config;
			Types::Board::PtrType _board;
			Types::Factory::PtrType _factory;

			Types::ScoreRenderer::PtrType _scoreRenderer;
			Types::ScoreRenderer::PtrType _movesCountRenderer;
			float _titleAnimationTime = 0.0f;
		};
	}
}

