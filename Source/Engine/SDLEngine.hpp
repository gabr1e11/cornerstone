//
// SDLEngine.hpp
//
// @author Roberto Cano
//

#pragma once

#include <optional>
#include <map>

#include "Framework/Core/Engine.hpp"
#include "Framework/Core/Types.hpp"

#include <sdl/SDL.h>

namespace SDL
{
#pragma region - Font
	struct Glyph {
		int id;
		int x;
		int y;
		int width;
		int height;
		int xoffset;
		int yoffset;
		int advance;
	};

	inline bool operator<(const Glyph& glyph, int id) { return glyph.id < id; }
	inline bool operator<(int id, const Glyph& glyph) { return id < glyph.id; }

	extern Glyph Font[191];

	class Engine;
	using EngineOwner = std::shared_ptr<Engine>;
	using EngineRef = Engine & ;

	class Engine final : public Framework::Core::Engine
	{
	public:
		using TextureId = uint32_t;
		using Size2D = Framework::Types::Size2D;

		static EngineOwner Create(const Settings& settings);

		~Engine() override = default;

		void start(Framework::Types::GameObject::PtrType updater) override;
		void stop() override;

		Point2D getMousePosition() const override;
		bool isMouseButtonDown() const override;
		Size getScreenSize() const override;
		float getElapsedTime() const override;

		TextureId registerTexture(const std::string& assetPath) override;
		Size getTextureSize(TextureId spriteId) const override;
		Size2D getTextSize(const std::string& text, float scale) const override;

		void render(TextureId spriteId, const Point2D& position, int zIndex = 0, const Color3D& color = Color3D(1.0f, 1.0f, 1.0f), float rotation = 0.0f, float scale = 1.0f, float opacity = 1.0f) override;
		void renderText(const std::string& text, const Point2D& position, int zIndex = 0, const Color3D& color = Color3D(1.0f, 1.0f, 1.0f), float rotation = 0.0f, float scale = 1.0f, float opacity = 1.0f) override;
		void renderText(const std::string& text, const Point2D& position, const std::vector<float>& yPositions, int zIndex = 0, const Color3D& color = Color3D(1.0f, 1.0f, 1.0f), float rotation = 0.0f, float scale = 1.0f, float opacity = 1.0f) override;

	protected:
		template<class _Ty>
		friend class std::_Ref_count_obj;
		Engine(const Settings& settings);

	private:
		struct RenderCommand
		{
			RenderCommand(TextureId textureId_, const Point2D& position_, const Color3D& color_, float rotation_, float scale_, float opacity_)
				: textureId(textureId_)
				, position(position_)
				, color(color_)
				, rotation(rotation_)
				, scale(scale_)
				, opacity(opacity_)
			{}
			RenderCommand(const std::string& text_, const Point2D& position_, const Color3D& color_, float rotation_, float scale_, float opacity_)
				: text(text_)
				, position(position_)
				, color(color_)
				, rotation(rotation_)
				, scale(scale_)
				, opacity(opacity_)
			{}
			RenderCommand(const std::string& text_, const Point2D& position_, const std::vector<float> yOffsets, const Color3D& color_, float rotation_, float scale_, float opacity_)
				: text(text_)
				, position(position_)
				, yOffsets(yOffsets)
				, color(color_)
				, rotation(rotation_)
				, scale(scale_)
				, opacity(opacity_)
			{}

			std::optional<TextureId> textureId;
			std::optional<std::string> text;

			Point2D position;
			std::vector<float> yOffsets;
			Color3D color;
			float scale = 1.0f;
			float rotation = 0.0f;
			float opacity = 0.0f;
		};

#pragma region - Texture handling
		void destroyTexture(SDL_Surface* texture);
		std::optional<TextureId> findTextureByAsset(const std::string& assetPath);
		SDL_Surface& getTexture(TextureId textureId) const;

		void renderCommandList();
		void handleMouseEvents();

#pragma region - Text/font handling
		const int InterLetterAdvance = 3;

		float getCharacterAdvance(char character, float scale) const;
		Glyph& findGlyph(char c) const;

#pragma region - Rendering
		void render(TextureId textureId, float x, float y, const glm::vec3& color, float rotation, float scale, float opacity);
		void render(TextureId textureId, const glm::mat4& transform, const glm::vec3& color, float opacity);
		void renderText(const char* text, const glm::mat4& transform, const std::vector<float>& yOffsets, const glm::vec3& color, float opacity);
		void renderText(const char* text, float x, float y, const std::vector<float>& yOffsets, const glm::vec3& color, float rotation, float scale, float opacity);

		std::string getFullPathFromAssetName(const std::string& assetName) const;

		void* _GLContext = nullptr;
		TextureId _fontSurfaceTextureId = 0;
		std::unordered_map<TextureId, SDL_Surface*> _textureMapById;
		std::unordered_map<std::string, TextureId> _textureMapByAssetName;
		std::multimap<int, RenderCommand, std::less<int>> _renderCommands;

		std::string _assetsDirectoryPath;
		Framework::Types::GameObject::PtrType _updater;

		SDL_Window* _window = nullptr;
		Size _windowSize;

		bool _isLoopRunning = false;
		float _elapsedTicks = 0.0f;
		float _lastFrameSeconds = 0.0f;
		Size2D _mousePosition;
		bool _isMouseButtonDown = false;
	};
}