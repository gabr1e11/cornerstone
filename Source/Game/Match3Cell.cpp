//
// Match3Cell.hpp
//
// @author Roberto Cano
//

#pragma once

#include "Match3Cell.hpp"

#include "Framework/Core/Types.hpp"
#include "Framework/Components/SpriteRenderer.hpp"

using namespace Match3::Game;
using Point2D = Framework::Types::Point2D;

namespace CellConstants
{
	const Point2D CellAnchorPoint(0.0f, 0.0f);
}

Cell::Cell(const Config& cellConfig)
{
	setSize(cellConfig.size);

	_stateMap = cellConfig.stateMap;
}

void Cell::Cell::init()
{
	addSpriteRenderer();
}

void Cell::Cell::setState(State state)
{
	_state = state;

	updateSpriteRendererWithState();
}

Cell::Cell::State Cell::Cell::getState() const
{
	return _state;
}

void Cell::Cell::addSpriteRenderer()
{
	const std::string& assetPath = getAssetPathForCurrentState();

	_spriteRenderer = CreateComponent<Framework::Components::SpriteRenderer>(assetPath);
	_spriteRenderer->setAnchorPoint(CellConstants::CellAnchorPoint);

	addComponent(_spriteRenderer);
}

void Cell::Cell::updateSpriteRendererWithState()
{
	const std::string& assetPath = getAssetPathForCurrentState();
	_spriteRenderer->setSprite(assetPath);
}

const std::string& Cell::Cell::getAssetPathForCurrentState() const
{
	return _stateMap.at(_state);
}