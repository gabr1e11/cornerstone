//
// SDLEngine.cpp
//
// @author Roberto Cano
//

#include "SDLEngine.hpp"

#include "Framework/Core/GameObject.hpp"

#include <stdexcept>
#include <algorithm>

#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glew/glew.h>
#include <glm/glm.hpp>

#include <sdl/Sdl.h>
#include <sdl/SDL_image.h>

using namespace SDL;

namespace SDLEngineConstants
{
	const float MaxFrameTicks = 300.0f;
}

EngineOwner Engine::Create(const Settings& settings)
{
	return std::make_shared<Engine>(settings);
}

Engine::Engine(const Settings& settings)
	: _assetsDirectoryPath(settings.assetsDirectoryPath)
	, _lastFrameSeconds(1.0f / 60.0f)
	, _windowSize(settings.windowSize)
	, _mousePosition(settings.windowSize.width * 0.5f, settings.windowSize.height * 0.5f)
	, _isMouseButtonDown(false)
	, _isLoopRunning(false)
	, _updater(nullptr)
	, _elapsedTicks(static_cast<float>(SDL_GetTicks()))
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_NOPARACHUTE) != 0) {
		throw std::runtime_error("Failed to init SDL");
	}

	_window = SDL_CreateWindow("SDLEngine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, _windowSize.width, _windowSize.height, SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL);
	if (_window == nullptr) {
		throw std::runtime_error(std::string("Error creating window: ") + SDL_GetError());
	}

	_GLContext = SDL_GL_CreateContext(_window);
	const char* error = SDL_GetError();
	if (*error != '\0') {
		throw std::runtime_error(std::string("Error initialising OpenGL context: ") + error);
	}

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetSwapInterval(1);

	glEnable(GL_TEXTURE_2D);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, settings.windowSize.width, settings.windowSize.height, 0.0f, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);

	_fontSurfaceTextureId = registerTexture(settings.fontFileName);
}

void Engine::start(Framework::Types::GameObject::PtrType updater)
{
	assert(updater);

	_updater = updater;

	_updater->setEngine(shared_from_this());
	callStart(_updater);

	SDL_ShowWindow(_window);

	while (!_isLoopRunning) {
		SDL_GL_SwapWindow(_window);
		glClear(GL_COLOR_BUFFER_BIT);

		handleMouseEvents();

		float currentTicks = static_cast<float>(SDL_GetTicks());
		float lastFrameTicks = currentTicks - _elapsedTicks;
		_elapsedTicks = currentTicks;

		lastFrameTicks = std::min(lastFrameTicks, SDLEngineConstants::MaxFrameTicks);
		_lastFrameSeconds = lastFrameTicks * 0.001f;

		callUpdate(updater, _lastFrameSeconds);

		renderCommandList();
	}
}

void Engine::stop()
{
	_isLoopRunning = true;
	_updater.reset();
}

float Engine::getElapsedTime() const
{
	return _lastFrameSeconds;
}

Engine::Point2D Engine::getMousePosition() const
{
	return _mousePosition;
}

bool Engine::isMouseButtonDown() const
{
	return _isMouseButtonDown;
}

std::string Engine::getFullPathFromAssetName(const std::string& assetName) const
{
	return _assetsDirectoryPath + assetName;
}

Engine::TextureId Engine::registerTexture(const std::string& assetPath)
{
	const std::string assetFullPath = getFullPathFromAssetName(assetPath);

	std::optional<TextureId> textureIdOpt = findTextureByAsset(assetFullPath);
	if (textureIdOpt != std::nullopt)
	{
		return textureIdOpt.value();
	}

	SDL_Surface* texture = IMG_Load(assetFullPath.c_str());
	if (texture == nullptr) {
		throw std::runtime_error(std::string("Unable to load texture ") + assetFullPath);
	}

	GLuint textureId;

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	int mode;
	switch (texture->format->BytesPerPixel) {
		case 4:
			mode = GL_RGBA;
			break;
		case 3:
			mode = GL_RGB;
			break;
		case 1:
			mode = GL_LUMINANCE_ALPHA;
			break;
		default:
			throw std::runtime_error("Image with unknown channel profile");
			break;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, mode, texture->w, texture->h, 0, mode, GL_UNSIGNED_BYTE, texture->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	_textureMapById.emplace(textureId, texture);

	return static_cast<TextureId>(textureId);
}

Engine::Size Engine::getTextureSize(TextureId textureId) const
{
	SDL_Surface& texture = getTexture(textureId);
	return Size(texture.w, texture.h);
}

Engine::Size Engine::getScreenSize() const
{
	return _windowSize;
}

Engine::Size2D Engine::getTextSize(const std::string& text, float scale) const
{
	Size2D textSize;

	int maxHeight = 0;
	int advance = 0;
	for (char letter : text) {
		Glyph& g = findGlyph(letter);
		advance += g.advance + InterLetterAdvance;
		maxHeight = std::max(maxHeight, g.height);
	}
	return Size2D(advance, maxHeight) * scale;
}

void Engine::render(TextureId textureId, const Point2D& position, int zIndex, const Color3D& color, float rotation, float scale, float opacity)
{
	_renderCommands.emplace(zIndex, RenderCommand(textureId, position, color, rotation, scale, opacity));
}

void Engine::renderText(const std::string& text, const Point2D& position, int zIndex, const Color3D& color, float rotation, float scale, float opacity)
{
	_renderCommands.emplace(zIndex, RenderCommand(text, position, color, rotation, scale, opacity));
}

void Engine::renderText(const std::string& text, const Point2D& position, const std::vector<float>& yOffsets, int zIndex, const Color3D& color, float rotation, float scale, float opacity)
{
	_renderCommands.emplace(zIndex, RenderCommand(text, position, yOffsets, color, rotation, scale, opacity));
}

void Engine::renderCommandList()
{
	for (const auto& cmdIter : _renderCommands)
	{
		const RenderCommand& cmd = cmdIter.second;

		if (cmd.textureId != std::nullopt)
		{
			render(cmd.textureId.value(), cmd.position.x, cmd.position.y, cmd.color, cmd.rotation, cmd.scale, cmd.opacity);
		}
		else if (cmd.text != std::nullopt)
		{
			renderText(cmd.text.value().c_str(), cmd.position.x, cmd.position.y, cmd.yOffsets, cmd.color, cmd.rotation, cmd.scale, cmd.opacity);
		}
	}

	_renderCommands.clear();
}

void Engine::render(TextureId textureId, float x, float y, const glm::vec3& color, float rotation, float scale, float opacity) {
	glm::mat4 transformation;
	transformation = glm::translate(transformation, glm::vec3(x, y, 0.0f));
	if (rotation) {
		transformation = glm::rotate(transformation, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	}
	if (scale)
	{
		transformation = glm::scale(transformation, glm::vec3(scale));
	}
	render(textureId, transformation, color, opacity);
}

void Engine::render(TextureId textureId, const glm::mat4& transform, const glm::vec3& color, float opacity) {
	glLoadMatrixf(reinterpret_cast<const float*>(&transform));

	SDL_Surface& surface = getTexture(textureId);

	glBindTexture(GL_TEXTURE_2D, textureId);

	GLint halfWidth = surface.w / 2;
	GLint halfHeight = surface.h / 2;

	// Alpha blending
	glColor4f(color.r, color.g, color.b, opacity);

	glBegin(GL_QUADS);
	glTexCoord2i(0, 1); glVertex2i(-halfWidth, halfHeight);
	glTexCoord2i(1, 1); glVertex2i(halfWidth, halfHeight);
	glTexCoord2i(1, 0); glVertex2i(halfWidth, -halfHeight);
	glTexCoord2i(0, 0); glVertex2i(-halfWidth, -halfHeight);
	glEnd();
}

void Engine::renderText(const char* text, const glm::mat4& transform, const std::vector<float>& yOffsets, const glm::vec3& color, float opacity) {
	glLoadMatrixf(reinterpret_cast<const float*>(&transform));
	int advance = 0;
	int offsetIndex = 0;

	SDL_Surface& fontSurface = getTexture(_fontSurfaceTextureId);

	for (; *text; ++text) {
		Glyph& g = findGlyph(*text);

		float yOffset = 0.0f;
		if (!yOffsets.empty())
		{
			yOffset = yOffsets.at(offsetIndex++);
		}

		float fontTexWidth = static_cast<float>(fontSurface.w);
		float fontTexHeight = static_cast<float>(fontSurface.h);

		float uvLeft = static_cast<float>(g.x) / fontTexWidth;
		float uvRight = static_cast<float>(g.x + g.width) / fontTexWidth;
		float uvBottom = static_cast<float>(g.y) / fontTexHeight;
		float uvTop = static_cast<float>(g.y + g.height) / fontTexHeight;

		float worldLeft = static_cast<float>(g.xoffset + advance);
		float worldRight = static_cast<float>(g.xoffset + g.width + advance);
		float worldBottom = static_cast<float>(g.yoffset + yOffset);
		float worldTop = static_cast<float>(g.yoffset + g.height + yOffset);

		glBindTexture(GL_TEXTURE_2D, _fontSurfaceTextureId);

		// Alpha blending
		glColor4f(color.r, color.g, color.b, opacity);

		glBegin(GL_QUADS);
		glTexCoord2f(uvLeft, uvTop); glVertex2f(worldLeft, worldTop);
		glTexCoord2f(uvRight, uvTop); glVertex2f(worldRight, worldTop);
		glTexCoord2f(uvRight, uvBottom); glVertex2f(worldRight, worldBottom);
		glTexCoord2f(uvLeft, uvBottom); glVertex2f(worldLeft, worldBottom);
		glEnd();

		advance += g.advance + InterLetterAdvance;
	}
}

void Engine::renderText(const char* text, float x, float y, const std::vector<float>& yOffsets, const glm::vec3& color, float rotation, float scale, float opacity) {
	Size2D textSize = getTextSize(text, scale);

	float xPos = x - textSize.x / 2.0f;
	float yPos = y - textSize.y / 2.0f;

	glm::mat4 transformation;
	transformation = glm::translate(transformation, glm::vec3(xPos, yPos, 0.0f));
	if (rotation) {
		transformation = glm::rotate(transformation, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	}
	if (scale)
	{
		transformation = glm::scale(transformation, glm::vec3(scale));
	}
	renderText(text, transformation, yOffsets, color, opacity);
}

void Engine::handleMouseEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
			case SDL_QUIT:
			case SDL_KEYDOWN:
				_isLoopRunning = true;
				break;
			case SDL_MOUSEBUTTONDOWN:
				_isMouseButtonDown = true;
				break;
			case SDL_MOUSEBUTTONUP:
				_isMouseButtonDown = false;
				break;
			case SDL_MOUSEMOTION:
				_mousePosition = Size2D(static_cast<float>(event.motion.x), static_cast<float>(event.motion.y));
				break;
			default:
				break;
		}
	}
}
SDL_Surface& Engine::getTexture(TextureId textureId) const
{
	auto& textureIter = _textureMapById.find(textureId);
	assert(textureIter != _textureMapById.end());

	return *(textureIter->second);
}

void Engine::destroyTexture(SDL_Surface* texture)
{
	SDL_FreeSurface(texture);
}

std::optional<Engine::TextureId> Engine::findTextureByAsset(const std::string& assetPath)
{
	const std::string assetFullPath = getFullPathFromAssetName(assetPath);

	const auto &assetTextureIdIter = _textureMapByAssetName.find(assetFullPath);
	if (assetTextureIdIter != _textureMapByAssetName.end())
	{
		return assetTextureIdIter->second;
	}
	return std::nullopt;
}

float Engine::getCharacterAdvance(char character, float scale) const
{
	Glyph& g = findGlyph(character);
	return static_cast<float>(g.advance + InterLetterAdvance) * scale;
}

Glyph& Engine::findGlyph(char c) const
{
	auto found = std::lower_bound(std::begin(Font), std::end(Font), c);
	if (found == std::end(Font) || c < *found) {
		found = std::lower_bound(std::begin(Font), std::end(Font), static_cast<int>('_'));
	}
	return *found;
}