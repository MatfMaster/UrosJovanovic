#include "Behaviour.h"

GameEngine::Behaviour::Behaviour(GameObject* gameObject)
	: Component(gameObject),
	  _enabled(true) {}

GameEngine::Behaviour::~Behaviour() {}

GameEngine::Component::ComponentType GameEngine::Behaviour::type() const
{
	return T_BEHAVIOUR;
}

void GameEngine::Behaviour::startUp() {}

void GameEngine::Behaviour::update(double deltaTime) {}

void GameEngine::Behaviour::shutDown() {}

bool GameEngine::Behaviour::isEnabled() const
{
	return _enabled;
}

void GameEngine::Behaviour::enable()
{
	_enabled = true;
}

void GameEngine::Behaviour::disable()
{
	_enabled = false;
}
