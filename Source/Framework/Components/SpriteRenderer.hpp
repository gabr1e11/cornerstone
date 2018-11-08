//
// SpriteRenderer.cpp
//
// @author Roberto Cano
//

#pragma once

#include "Framework/Core/Types.hpp"
#include "Framework/Core/GameComponent.hpp"
#include "Framework/Core/AnchorPointUser.hpp"
#include "CodeExecution.hpp"

namespace Framework
{
	using namespace Types;
	namespace Components
	{
		class SpriteRenderer : public Core::GameComponent, public Core::AnchorPointUser
		{
		public:
			SpriteRenderer(const std::string& assetPath);

			TextureId getSpriteId() const;

			void init() override;
			void update(float dt) override;

			const Size& getSize() const;

			void setSprite(const std::string& assetPath);
			const std::string& getSprite();

			// TODO: Extract all this to Transform
			void setRotation(float radians);
			float getRotation() const;

			void setOpacity(float opacity);
			float getOpacity() const;

			void setScale(float scale);
			float getScale() const;

			void setVisible(bool visible);
			bool isVisible() const;

			void setZIndexOffset(int offset);
			int getZIndexOffset() const;

		private:
			void renderSprite();

			std::string _assetPath;
			TextureId _id = -1;
			Size _size;
			Point2D _anchorPoint = Point2D(0.0f, 0.0f);
			float _rotation = 0.0f;
			float _scale = 1.0f;
			float _opacity = 1.0f;
			bool _isVisible = true;
			int _zIndexOffset = 0;
		};
	}
}

