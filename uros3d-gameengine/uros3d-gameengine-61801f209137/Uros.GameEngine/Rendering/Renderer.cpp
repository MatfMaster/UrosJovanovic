#include "Renderer.h"
#include "GameObject.h"

GameEngine::Renderer::Renderer(GameObject* gameObject)
	: Component(gameObject),
	  _enabled(true)
{
	connect(this->gameObject()->transform(), SIGNAL(changed(Transform*)),
	        this, SLOT(onTransformChanged(Transform*)));
}

GameEngine::Renderer::~Renderer() {}

GameEngine::Component::ComponentType GameEngine::Renderer::type() const
{
	return T_RENDERER;
}

bool GameEngine::Renderer::isSingular() const
{
	return true;
}

bool GameEngine::Renderer::isEnabled() const
{
	return _enabled;
}

GameEngine::Material& GameEngine::Renderer::getMaterial()
{
	return _material;
}

const GameEngine::Material& GameEngine::Renderer::getConstMaterial() const
{
	return _material;
}

void GameEngine::Renderer::enable()
{
	_enabled = true;
}

void GameEngine::Renderer::disable()
{
	_enabled = false;
}

void GameEngine::Renderer::setMaterial(const Material& material)
{
	_material = material;
}

void GameEngine::Renderer::onTransformChanged(Transform* transform) {}
