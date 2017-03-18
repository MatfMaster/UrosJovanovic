#include "RotateAround.h"

RotateAround::RotateAround(GameEngine::GameObject* gameObject)
	: Behaviour(gameObject) { }

RotateAround::~RotateAround() { }

void RotateAround::update(double deltaTime)
{
	gameObject()->transform()->rotate(GameEngine::Transform::up(), 1);
}
