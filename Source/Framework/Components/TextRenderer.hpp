//
// TextRenderer.hpp
//
// @author Roberto Cano
//

#pragma once

#include "Framework/Core/GameComponent.hpp"

namespace Framework
{
	namespace Components
	{
		class TextRenderer : public Core::GameComponent
		{
		public:
			using Point2D = Framework::Types::Point2D;
			using Color3D = Framework::Types::Color3D;

			TextRenderer() = default;
			~TextRenderer() override = default;

			void setRotation(float rotation);
			float getRotation() const;

			void setScale(float scale);
			float getScale() const;

			void setOpacity(float opacity);
			float getOpacity() const;

			void setPosition(const Point2D& newPosition);
			const Point2D& getPosition() const;

			void setText(const std::string& text);
			const std::string& getText() const;

			void setColor(const Color3D& color);
			const Color3D& getColor() const;

			void update(float dt) override;

		private:
			enum class State
			{
				Stopped,
				Playing
			};

			void renderText();

			// TODO: transform
			Point2D _position;
			float _rotation = 0.0f;
			float _scale = 1.0f;
			float _opacity = 1.0f;
			Color3D _color;

			std::string _text;
		};
	}
}
