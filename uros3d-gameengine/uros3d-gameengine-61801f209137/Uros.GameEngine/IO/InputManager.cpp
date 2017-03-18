#include <QtGui/QKeyEvent>
#include "InputManager.h"

GameEngine::InputManager::InputManager()
	: _key(0),
	  _keyUp(0),
	  _keyDown(0),
	  _modifiers(0),
	  _mouseButton(0),
	  _mouseButtons(0),
	  _mouseButtonUp(0),
	  _mouseButtonDown(0),
	  _mouseX(0),
	  _mouseY(0),
	  _lastMouseX(0),
	  _lastMouseY(0),
	  _mouseXAxis(0),
	  _mouseYAxis(0),
	  _lastMouseWheelAxis(0),
	  _mouseWheelAxis(0)
{
	connect(&_updateTimer, SIGNAL(timeout()), this, SLOT(update()));
	_updateTimer.start(20); //Do not change!
}

GameEngine::InputManager::~InputManager() {}

bool GameEngine::InputManager::eventFilter(QObject* object, QEvent* event)
{
	if (event->type() == QEvent::KeyPress)
	{
		auto keyEvent = static_cast<QKeyEvent*>(event);
		//DEBUG_LOG("InputManager::eventFilter() Key pressed : " << keyEvent->key());
		_key = keyEvent->key();
		_keyDown = keyEvent->key();
		_keyUp = 0;
		_keys.insert(_key);
		_modifiers = keyEvent->modifiers();
	}
	else if (event->type() == QEvent::KeyRelease)
	{
		auto keyEvent = static_cast<QKeyEvent*>(event);
		//DEBUG_LOG("InputManager::eventFilter() Key released : " << keyEvent->key());
		_key = keyEvent->key();
		_keyUp = keyEvent->key();
		_keyDown = 0;
		_keys.remove(_key);
		_modifiers = keyEvent->modifiers();
	}
	else if (event->type() == QEvent::MouseButtonPress)
	{
		auto mouseEvent = static_cast<QMouseEvent*>(event);
		//DEBUG_LOG("InputManager::eventFilter() Mouse button pressed : " << mouseEvent->buttons());
		_mouseButton = mouseEvent->button();
		_mouseButtons = mouseEvent->buttons();
		_mouseButtonDown = mouseEvent->button();
		_mouseButtonUp = 0;
	}
	else if (event->type() == QEvent::MouseButtonRelease)
	{
		auto mouseEvent = static_cast<QMouseEvent*>(event);
		//DEBUG_LOG("InputManager::eventFilter() Mouse button released : " << mouseEvent->button());
		_mouseButton = mouseEvent->button();
		_mouseButtonUp = mouseEvent->button();
		_mouseButtons = 0;
		_mouseButtonDown = 0;
	}
	else if (event->type() == QEvent::MouseMove)
	{
		auto mouseEvent = static_cast<QMouseEvent*>(event);
		//DEBUG_LOG("InputManager::eventFilter() Mouse move : " << mouseEvent->globalX() << " " << mouseEvent->globalY());
		_mouseX = mouseEvent->globalX();
		_mouseY = mouseEvent->globalY();
	}
	else if (event->type() == QEvent::Wheel)
	{
		auto wheelEvent = static_cast<QWheelEvent*>(event);
		//DEBUG_LOG("InputManager::eventFilter() Mouse wheel : " << wheelEvent->delta());
		_mouseWheelAxis = wheelEvent->delta();
		_mouseButtons = wheelEvent->buttons();
	}
	return false;
}

void GameEngine::InputManager::reset()
{
	_keyUp = 0;
	_mouseButtonUp = 0;
}

GameEngine::InputManager* GameEngine::InputManager::instance()
{
	//This is a C++11 feature
	//Object initialization will be made only by one thread,
	//other threads will wait till it complete
	//More: http://stackoverflow.com/questions/2576022/efficient-thread-safe-singleton-in-c
	//TODO: Do this for all singletons
	static InputManager instance;
	return &instance;
}

bool GameEngine::InputManager::keyPressed(int key) const
{
	return _keys.contains(key);
}

int GameEngine::InputManager::getKey() const
{
	return _key;
}

int GameEngine::InputManager::getKeyUp() const
{
	return _keyUp;
}

int GameEngine::InputManager::getKeyDown() const
{
	return _keyDown;
}

int GameEngine::InputManager::getModifiers() const
{
	return _modifiers;
}

int GameEngine::InputManager::getMouseButton() const
{
	return _mouseButton;
}

int GameEngine::InputManager::getMouseButtons() const
{
	return _mouseButtons;
}

int GameEngine::InputManager::getMouseButtonUp() const
{
	return _mouseButtonUp;
}

int GameEngine::InputManager::getMouseButtonDown() const
{
	return _mouseButtonDown;
}

int GameEngine::InputManager::getMouseX() const
{
	return _mouseX;
}

int GameEngine::InputManager::getMouseY() const
{
	return _mouseY;
}

float GameEngine::InputManager::getMouseXAxis() const
{
	return _mouseXAxis;
}

float GameEngine::InputManager::getMouseYAxis() const
{
	return _mouseYAxis;
}

float GameEngine::InputManager::getMouseWheelAxis() const
{
	return _mouseWheelAxis;
}

void GameEngine::InputManager::update()
{
	_mouseXAxis = _mouseX - _lastMouseX;
	_mouseYAxis = -(_mouseY - _lastMouseY);
	_lastMouseX = _mouseX;
	_lastMouseY = _mouseY;
	_mouseWheelAxis = _mouseWheelAxis - _lastMouseWheelAxis;
	_lastMouseWheelAxis = _mouseWheelAxis;
}
