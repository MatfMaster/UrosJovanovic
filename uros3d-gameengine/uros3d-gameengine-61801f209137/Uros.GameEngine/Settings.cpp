#include "Settings.h"

GameEngine::Settings::Settings()
	:_vSync(true),
	 _windowType(Window),
	 _antiAliasingType(Off),
	 _rendererType(OpenGL) {}

bool GameEngine::Settings::isVSyncEnabled() const
{
	return _vSync;
}

GameEngine::Settings::WindowType GameEngine::Settings::getWindowType() const
{
	return _windowType;
}

GameEngine::Settings::AntiAliasingType GameEngine::Settings::getAntiAliasingType() const
{
	return _antiAliasingType;
}

GameEngine::Settings::RendererType GameEngine::Settings::getRendererType() const
{
	return _rendererType;
}

void GameEngine::Settings::enableVSync()
{
	_vSync = true;
}

void GameEngine::Settings::disableVSync()
{
	_vSync = false;
}

void GameEngine::Settings::setWindowType(const WindowType& type)
{
	_windowType = type;
}

void GameEngine::Settings::setAntialiasingType(const AntiAliasingType& type)
{
	_antiAliasingType = type;
}

void GameEngine::Settings::setRendererType(const RendererType& type)
{
	_rendererType = type;
}
