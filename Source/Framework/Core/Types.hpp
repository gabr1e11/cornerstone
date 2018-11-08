//
// Types.hpp
//
// @author Roberto Cano
//

#pragma once

#include <string>
#include <functional>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace Framework
{
	namespace Core
	{
		class GameObject;
		class GameComponent;
		class Engine;
		class Scene;
	}
	namespace Components
	{
		class MouseHandler;
		class IMouseHandlerDelegate;
		class SpriteRenderer;
		class CodeExecution;
		class BounceUpDownAnimation;
		class SoundEmitter;
		class TextRenderer;
	}

	namespace Types
	{
		using ComponentId = std::string;
		using InstanceId = std::string;
		using InstanceIdInvalid = std::string();

		using Point2D = glm::vec2;
		using Direction2D = glm::vec2;
		using Direction = glm::ivec2;
		using Color3D = glm::vec3;

		using TextureId = uint32_t;

		using Size2D = glm::vec2;
		struct Size
		{
			Size(int width_, int height_) : width(width_), height(height_) {}
			Size() = default;

			operator Size2D()
			{
				return Size2D(width, height);
			}
			operator const Size2D() const
			{
				return Size2D(width, height);
			}

			int width = 0;
			int height = 0;
		};

		namespace GameObject
		{
			using PtrType = std::shared_ptr<Core::GameObject>;
			using WeakPtrType = std::weak_ptr<Core::GameObject>;
		}
		namespace GameComponent
		{
			using PtrType = std::shared_ptr<Core::GameComponent>;
			using WeakPtrType = std::weak_ptr<Core::GameComponent>;
		}
		namespace Engine
		{
			using PtrType = std::shared_ptr<Core::Engine>;
		}
		namespace Scene
		{
			using PtrType = std::shared_ptr<Core::Scene>;
			using WeakPtrType = std::weak_ptr<Core::Scene>;
		}
		namespace SpriteRenderer
		{
			using PtrType = std::shared_ptr<Components::SpriteRenderer>;
		}
		namespace MouseHandler
		{
			using PtrType = std::shared_ptr<Components::MouseHandler>;
		}
		namespace IMouseHandlerDelegate
		{
			using PtrType = std::shared_ptr<Components::IMouseHandlerDelegate>;
		}
		namespace CodeExecution
		{
			using PtrType = std::shared_ptr<Components::CodeExecution>;
		}
		namespace BounceUpDownAnimation
		{
			using PtrType = std::shared_ptr<Components::BounceUpDownAnimation>;
		}
		namespace SoundEmitter
		{
			using PtrType = std::shared_ptr<Components::SoundEmitter>;
		}
		namespace TextRenderer
		{
			using PtrType = std::shared_ptr<Components::TextRenderer>;
		}
		template<typename ...Args>
		using Callback = std::function<void(Args...)>;
	}
}
