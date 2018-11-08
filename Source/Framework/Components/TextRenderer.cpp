//
// TextRenderer.cpp
//
// @author Roberto Cano
//

#include "SoundEmitter.hpp"

#include "Framework/Core/Engine.hpp"
#include "Framework/Core/GameObject.hpp"
#include "Framework/Core/Types.hpp"
#include "TextRenderer.hpp"

using namespace Framework::Components;

void TextRenderer::setRotation(float rotation)
{
	_rotation = rotation;
}

float TextRenderer::getRotation() const
{
	return _rotation;
}

void TextRenderer::setScale(float scale)
{
	_scale = scale;
}

float TextRenderer::getScale() const
{
	return _scale;
}

void TextRenderer::setOpacity(float opacity)
{
	_opacity = opacity;
}

float TextRenderer::getOpacity() const
{
	return _opacity;
}

void TextRenderer::setPosition(const Point2D & newPosition)
{
	_position = newPosition;
}

const TextRenderer::Point2D & TextRenderer::getPosition() const
{
	return _position;
}

void TextRenderer::setText(const std::string& description)
{
	_text = description;
}

const std::string& TextRenderer::getText() const
{
	return _text;
}

void TextRenderer::setColor(const Color3D & color)
{
	_color = color;
}

const TextRenderer::Color3D& TextRenderer::getColor() const
{
	return _color;
}

void TextRenderer::update(float dt)
{
	renderText();
}

void TextRenderer::renderText()
{
	const auto& owner = getOwner<Framework::Core::GameObject>();

	const Point2D& parentPosition = owner.getWorldPosition();
	const Point2D renderPosition = parentPosition + getPosition();

	getEngine()->renderText(_text, renderPosition, owner.getZIndex(), _color, _rotation, _scale, _opacity);
}