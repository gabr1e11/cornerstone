//
// SpriteRenderer.cpp
//
// @author Roberto Cano
//

#pragma once

#include "SpriteRenderer.hpp"

#include "Framework/Core/Types.hpp"
#include "Framework/Core/GameObject.hpp"
#include "Framework/Core/Engine.hpp"

using namespace Framework;
using namespace Framework::Core;
using namespace Framework::Types;

namespace SpriteRendererConstants
{
	const Point2D RendererDefaultAnchorPoint = Point2D(0.5f, 0.5f);
}

Components::SpriteRenderer::SpriteRenderer(const std::string& assetPath)
	: _assetPath(assetPath)
{
	// Empty on purpose
}

TextureId Components::SpriteRenderer::getSpriteId() const
{
	return _id;
}

void Components::SpriteRenderer::init()
{
	setSprite(_assetPath);
}

void Components::SpriteRenderer::update(float dt)
{
	if (isVisible())
	{
		renderSprite();
	}
}

const Size& Components::SpriteRenderer::getSize() const
{
	return _size;
}

void Components::SpriteRenderer::setRotation(float radians)
{
	_rotation = radians;
}

float Components::SpriteRenderer::getRotation() const
{
	return _rotation;
}

void Components::SpriteRenderer::setOpacity(float opacity)
{
	_opacity = opacity;
}

float Components::SpriteRenderer::getOpacity() const
{
	return _opacity;
}

void Components::SpriteRenderer::setScale(float scale)
{
	_scale = scale;
}

float Components::SpriteRenderer::getScale() const
{
	return _scale;
}

void Components::SpriteRenderer::setVisible(bool visible)
{
	_isVisible = visible;
}

bool Components::SpriteRenderer::isVisible() const
{
	return _isVisible;
}

void Components::SpriteRenderer::setZIndexOffset(int offset)
{
	_zIndexOffset = offset;
}

int Components::SpriteRenderer::getZIndexOffset() const
{
	return _zIndexOffset;
}

void Components::SpriteRenderer::setSprite(const std::string& assetPath)
{
	_assetPath = assetPath;

	_id = getEngine()->registerTexture(_assetPath);
	_size = getEngine()->getTextureSize(_id);
}

const std::string& Components::SpriteRenderer::getSprite()
{
	return _assetPath;
}

void Components::SpriteRenderer::renderSprite()
{
	using namespace SpriteRendererConstants;

	const Core::GameObject& gameObject = getOwner();

	const Point2D& goWorldPosition = gameObject.getWorldPosition();

	const Size2D spriteSize = _size;
	const Point2D anchorPoint = getAnchorPoint();

	Point2D anchorOffset = (RendererDefaultAnchorPoint - anchorPoint) * spriteSize;

	const Point2D renderPosition = goWorldPosition + anchorOffset;

	getEngine()->render(_id, renderPosition, gameObject.getZIndex() + _zIndexOffset, Color3D(1.0f, 1.0f, 1.0f), _rotation, _scale, _opacity);
}