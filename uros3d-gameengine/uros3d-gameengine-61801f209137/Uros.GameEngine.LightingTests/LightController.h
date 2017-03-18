#pragma once
#include "Behaviour.h"
#include "Scene/Light.h"

#define Input (*GameEngine::InputManager::instance())

class LightController : public GameEngine::Behaviour
{
	GameEngine::Light* _light;
public:
	LightController(GameEngine::GameObject* gameObject);
	void update(double deltaTime) override;
};