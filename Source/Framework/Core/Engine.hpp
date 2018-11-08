//
// Engine.hpp
//
// @author Roberto Cano
//

#pragma once

#include <string>

#include "Types.hpp"

namespace Framework
{
	namespace Core
	{
		class Engine : public std::enable_shared_from_this<Engine>
		{
		public:
			using Point2D = Framework::Types::Point2D;
			using Size = Framework::Types::Size;
			using Size2D = Framework::Types::Size2D;
			using TextureId = Framework::Types::TextureId;
			using Color3D = Framework::Types::Color3D;

			struct Settings
			{
				Size windowSize;
				std::string assetsDirectoryPath = std::string("assets/");
				std::string fontFileName = "berlin_sans_demi_72_0.png";

				void validate() const;
			};

			virtual ~Engine() = default;

			virtual void start(Types::GameObject::PtrType updater) = 0;
			virtual void stop() = 0;

			virtual Point2D getMousePosition() const = 0;
			virtual bool isMouseButtonDown() const = 0;
			virtual Size getScreenSize() const = 0;
			virtual float getElapsedTime() const = 0;

			virtual TextureId registerTexture(const std::string& assetPath) = 0;
			virtual Size getTextureSize(TextureId spriteId) const = 0;
			virtual Size2D getTextSize(const std::string& text, float scale) const = 0;
			virtual float getCharacterAdvance(const char character, float scale = 1.0f) const = 0;

			virtual void render(TextureId spriteId, const Point2D& position, int zIndex = 0, const Color3D& color = Color3D(1.0f, 1.0f, 1.0f), float rotation = 0.0f, float scale = 1.0f, float opacity = 1.0f) = 0;
			virtual void renderText(const std::string& text, const Point2D& position, int zIndex = 0, const Color3D& color = Color3D(1.0f, 1.0f, 1.0f), float rotation = 0.0f, float scale = 1.0f, float opacity = 1.0f) = 0;
			virtual void renderText(const std::string& text, const Point2D& position, const std::vector<float>& yPositions, int zIndex = 0, const Color3D& color = Color3D(1.0f, 1.0f, 1.0f), float rotation = 0.0f, float scale = 1.0f, float opacity = 1.0f) = 0;

		protected:
#pragma region - Convenience methods for derived class
			void callInit(Framework::Types::GameObject::PtrType gameObject);
			void callStart(Framework::Types::GameObject::PtrType gameObject);
			void callUpdate(Framework::Types::GameObject::PtrType gameObject, float dt);
		};
	}
}