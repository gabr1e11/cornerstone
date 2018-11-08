//
// Match3Config.hpp
//
// @author Roberto Cano
//

#pragma once

#include "Framework/Core/Types.hpp"
#include "Framework/Core/Engine.hpp"

#include "Match3Symbol.fwd.hpp"
#include "Match3Cell.fwd.hpp"
#include "Match3Utils.hpp"

namespace Match3
{
	namespace Game
	{
		struct Config
		{
			using Size = Framework::Types::Size;
			using SymbolType = Match3::Types::Symbol::Type;
			using CellType = Match3::Types::Cell::State;
			using BoardLevelData = Match3::Utils::Matrix2x2<CellType>;

			struct SymbolDefinition
			{
				SymbolDefinition(SymbolType type_, const std::string& avatarAssetName_, const std::string& shadowAssetName_, float dropRate_)
					: type(type_)
					, avatarAssetName(avatarAssetName_)
					, shadowAssetName(shadowAssetName_)
					, dropRate(dropRate_)
				{
					// Empty on purpose
				}
				SymbolDefinition() = default;

				SymbolType type = SymbolType::_end;
				std::string avatarAssetName;
				std::string shadowAssetName;
				float dropRate = 0.0f;
			};
			using SymbolDefinitions = std::unordered_map<SymbolType, SymbolDefinition>;

			struct BoardCellDefinition
			{
				BoardCellDefinition(CellType type_, const std::string& cellAssetName_)
					: type(type_)
					, cellAssetName(cellAssetName_)
				{
					// Empty on purpose
				}
				BoardCellDefinition() = default;

				CellType type = CellType::Active;
				std::string cellAssetName;
			};

			using BoardCellDefinitions = std::unordered_map<CellType, BoardCellDefinition>;

			struct BoardLevel
			{
				BoardCellDefinitions boardCellDefinitions;
				BoardLevelData levelConfig;
			};
			using BoardLevels = std::vector<BoardLevel>;

#pragma region - Game definition
			Framework::Core::Engine::Settings engineSettings;

			Size boardSizeInCells = Size(8, 8);
			Size cellSizeInPixels = Size(38, 38);

			std::string backgroundAsset = "background.png";
			SymbolDefinitions symbolDefinitions;

			BoardLevels levels;

			void validate() const;
		};
	}
}

