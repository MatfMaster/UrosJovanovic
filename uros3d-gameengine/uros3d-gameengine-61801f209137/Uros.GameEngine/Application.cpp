#include "Application.h"
#include "IO/InputManager.h"
#include "ProjectManager.h"
#include "Rendering/Viewport.h"
#include "Rendering/OpenGL/ViewportGL.h"
#include "Rendering/RenderingManager.h"
#include "Rendering/OpenGL/RenderingManagerOGL.h"

GameEngine::Settings GameEngine::Application::_settings = Settings();
GameEngine::Viewport* GameEngine::Application::_viewport = nullptr;
bool GameEngine::Application::_running = false;

GameEngine::Application::Application() {}

GameEngine::Application::~Application() {}

bool GameEngine::Application::initialize(QCoreApplication& application, const Settings& settings)
{
	// Init renderer
	RenderingManagerInstance* renderer;
	switch (settings.getRendererType())
	{
		case Settings::Automatic:
			renderer = new RenderingManagerOGL();
			break;
		case Settings::OpenGL:
			renderer = new RenderingManagerOGL();
			break;
		case Settings::Direct3D:
			// No D3D support yet... :(
			ERROR_LOG("> Application::initialize() Failed to initialize the engine: Unsupported renderer type (D3D).");
			return false;
		default:
			renderer = nullptr;
			break;
	}

	RenderingManager::initialize(renderer);

	// Init input management
	application.installEventFilter(InputManager::instance());

	_settings = settings;
	return true;
}

bool GameEngine::Application::isRunning()
{
	return _running;
}

void GameEngine::Application::run()
{
	if (_running)
	{
		ERROR_LOG("> Application::run() Game Engine is already running.");
		return;
	}

	QSurfaceFormat format;
	format.setDepthBufferSize(24);

	// AntiAliasing
	int samples;
	switch (_settings.getAntiAliasingType())
	{
		case Settings::MSAAx4:
			samples = 4;
			break;
		case Settings::MSAAx8:
			samples = 8;
			break;
		case Settings::MSAAx16:
			samples = 16;
			break;
		default:
			samples = 0;
			break;
	}
	format.setSamples(samples);

	// VSync
	if (!_settings.isVSyncEnabled())
		format.setSwapInterval(0);

	if (_viewport)
		delete _viewport;

	_viewport = new ViewportGL();
	_viewport->setFormat(format);
	_viewport->setTitle(ProjectManager::instance()->activeProject()->getName());
	_viewport->resize(800, 600);
	switch (_settings.getWindowType())
	{
		case Settings::Window:
			_viewport->show();
			break;
		case Settings::WindowBorderless:
			_viewport->setFlags(Qt::FramelessWindowHint);
			_viewport->show();
			break;
		case Settings::Fullscreen:
			_viewport->showFullScreen();
			break;
		default:
			break;
	}

	//TODO: This should be moved somewhere else...
	if (auto project = ProjectManager::instance()->activeProject())
		if (auto scene = project->getActiveScene())
			scene->initialize();

	_running = true;
}

GameEngine::Settings& GameEngine::Application::settings()
{
	return _settings;
}

GameEngine::Viewport* GameEngine::Application::viewport()
{
	return _viewport;
}
