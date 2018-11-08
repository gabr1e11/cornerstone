//
// main.cpp
//
// @author Roberto Cano
//

#include "Match3MainScene.hpp"

#include "Framework/Core/Factory.hpp"
#include "Engine/SDLEngine.hpp"

#include "Match3Symbol.fwd.hpp"
#include "Match3Cell.fwd.hpp"

int main(int argc, char *argv[])
{
	using namespace Match3::Game;

	using SymbolType = Match3::Types::Symbol::Type;
	using CellType = Match3::Types::Cell::State;
	using CellPosition = Match3::Types::Cell::Position;

	Config config;

	config.engineSettings.windowSize = Framework::Types::Size(540, 720);
	config.engineSettings.assetsDirectoryPath = "assets/";
	config.engineSettings.fontFileName = "berlin_sans_demi_72_0.png";

	config.boardSizeInCells = Framework::Types::Size(8, 8);
	config.cellSizeInPixels = Framework::Types::Size(65, 65);

	config.backgroundAsset = "background.png";

	config.symbolDefinitions[SymbolType::Donut] = Config::SymbolDefinition(SymbolType::Donut, "donut-symbol.png", "donut-shadow.png", 0.2f);
	config.symbolDefinitions[SymbolType::Cookie] = Config::SymbolDefinition(SymbolType::Cookie, "cookie-symbol.png", "cookie-shadow.png", 0.2f);
	config.symbolDefinitions[SymbolType::Candy] = Config::SymbolDefinition(SymbolType::Candy, "candy-symbol.png", "candy-shadow.png", 0.2f);
	config.symbolDefinitions[SymbolType::Muffin] = Config::SymbolDefinition(SymbolType::Muffin, "muffin-symbol.png", "muffin-shadow.png", 0.2f);
	config.symbolDefinitions[SymbolType::Pie] = Config::SymbolDefinition(SymbolType::Pie, "pie-symbol.png", "pie-shadow.png", 0.2f);

	// Cell definitions
	Config::BoardLevel level1;

	level1.boardCellDefinitions[CellType::Normal] = Config::BoardCellDefinition(CellType::Normal, "background-cell.png");
	level1.boardCellDefinitions[CellType::Active] = Config::BoardCellDefinition(CellType::Active, "active-cell.png");
	level1.boardCellDefinitions[CellType::Disabled] = Config::BoardCellDefinition(CellType::Disabled, "disabled-cell.png");

	Config::BoardLevelData levelData(config.boardSizeInCells);

	for (int column = 0; column < config.boardSizeInCells.height; ++column)
	{
		for (int row = 0; row < config.boardSizeInCells.width; ++row)
		{
			CellPosition cellPosition(column, row);
			levelData.at(cellPosition) = CellType::Normal;
		}
	}

	level1.levelConfig = std::move(levelData);
	config.levels.push_back(std::move(level1));

	auto mainScene = Framework::Core::Factory::CreateScene<MainScene>(config);
	auto engine = SDL::Engine::Create(config.engineSettings);

	engine->start(mainScene);

	return 0;
}