//
// Engine.cpp
//
// @author Roberto Cano
//

#include "Engine.hpp"

#include "GameObject.hpp"

#include "Engine/SDLEngine.hpp"

using namespace Framework::Core;

void Engine::Settings::validate() const
{
	// Check that the assets path has a trailing '/'
	if (!assetsDirectoryPath.empty())
	{
		assert(assetsDirectoryPath.back() == '/' || assetsDirectoryPath.back() == '\\');
	}
}

void Engine::callInit(Framework::Types::GameObject::PtrType gameObject)
{
	gameObject->internalInit();
}

void Engine::callStart(Framework::Types::GameObject::PtrType gameObject)
{
	gameObject->internalStart();
}

void Engine::callUpdate(Framework::Types::GameObject::PtrType gameObject, float dt)
{
	gameObject->internalUpdate(dt);
}