//
// Match3Globals.hpp
//
// @author Roberto Cano
//

#pragma once

namespace Match3
{
	namespace Game
	{
		namespace Globals
		{
			const int BackgroundZIndex = -10;

			const int MainSceneZIndex = 0;

			const int CellZIndex = 5;

			const int SymbolNormalZIndex = 10;
			const int SymbolOnTopZIndex = 11;

			const int TextRenderingZIndex = 15;

			const int SymbolFlyingZIndex = 20;

			const int MinNumberMatches = 3;
			const int MaxBoardSize = 1000;

			const std::string SymbolSpriteRendererName = "SymbolRenderer";
			const std::string ShadowSpriteRendererName = "ShadowRenderer";
		}
	}
}
