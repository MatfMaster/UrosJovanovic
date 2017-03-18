#include "Component.h"
#include "Gameobject.h"

GameEngine::Component::Component(GameObject* gameObject)
{
	_gameObject = gameObject;
}

GameEngine::Component::~Component()
{
	_gameObject->removeComponent(this);
}

GameEngine::GameObject* GameEngine::Component::gameObject() const
{
	return _gameObject;
}

bool GameEngine::Component::isSingular() const
{
	return false;
}

void GameEngine::Component::destroy(Component* component)
{
	delete component;
}
