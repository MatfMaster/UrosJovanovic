#include "GameObject.h"
#include "IO/InputManager.h"
#include "LightController.h"

using namespace GameEngine;

LightController::LightController(GameObject* gameObject)
: Behaviour(gameObject)
{
	_light = gameObject->getComponent<Light>();
}

void LightController::update(double deltaTime)
{
	int vertical = 0;
	int horizontal = 0;

	//auto keyDown = Input.getKeyDown();

	if (Input.keyPressed(Qt::Key_W))
		vertical = 1;
	if (Input.keyPressed(Qt::Key_S))
		vertical = -1;
	if (Input.keyPressed(Qt::Key_A))
		horizontal = 1;
	if (Input.keyPressed(Qt::Key_D))
		horizontal = -1;

	gameObject()->transform()->translate(vertical * Transform::forward() * 0.1);
	gameObject()->transform()->translate(horizontal * Transform::right() * 0.1);
}