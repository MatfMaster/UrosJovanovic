#include <QtGui/QKeyEvent>
#include <QCoreApplication>
#include "Viewport.h"
#include "Scene/Camera.h"
#include "ProjectManager.h"
#include "Rendering/RenderingManager.h"
#include "IO/InputManager.h"

GameEngine::Viewport::Viewport(QWindow* parent)
	: QWindow(parent),
	  initialized(false)
{
	updateTimer.start();
	connect(&statsUpdateTimer, SIGNAL(timeout()), this, SLOT(onStatsUpdateTimerTimeout()));
	statsUpdateTimer.start(1000);
}

GameEngine::Viewport::~Viewport() {}

bool GameEngine::Viewport::event(QEvent* e)
{
	if (e->type() == QEvent::UpdateRequest)
	{
		double elapsedSeconds = updateTimer.nsecsElapsed() / 10e9;

		// Update game logic
		updateTimer.restart();
		if (auto project = ProjectManager::instance()->activeProject())
			if (auto scene = project->getActiveScene())
				scene->update(elapsedSeconds);

		// Render everything
		render();

		// Now is a safe time to reset any temp input flags
		InputManager::instance()->reset();

		// Keep the loop spinning
		QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
		return true;
	}

	// TODO: Check if this is a safe place to call init funcs
	if (auto surfaceEvent = dynamic_cast<QPlatformSurfaceEvent*>(e))
		if (surfaceEvent->surfaceEventType() == QPlatformSurfaceEvent::SurfaceCreated)
			if (!initialized)
			{
				init();
				RenderingManager::instance()->initialize();
				resized(width(), height());
				initialized = true;
			}

	return QWindow::event(e);
}

void GameEngine::Viewport::exposeEvent(QExposeEvent* e)
{
	if (isExposed())
		requestUpdate();
}

void GameEngine::Viewport::resizeEvent(QResizeEvent* e)
{
	resized(e->size().width(), e->size().height());
}

void GameEngine::Viewport::render()
{
	onBeforeRender();

	if (auto project = ProjectManager::instance()->activeProject())
		if (auto scene = project->getActiveScene())
			scene->render();

	onAfterRender();
}

void GameEngine::Viewport::resized(int width, int height)
{
	onBeforeResize(width, height);

	if (auto project = ProjectManager::instance()->activeProject())
		if (auto scene = project->getActiveScene())
			for (auto camera : scene->cameras())
			{
				auto ratio = width * 1.0f / height;
				camera->setAspectRatio(ratio);
			}

	onAfterResize(width, height);
}

void GameEngine::Viewport::init() {}

void GameEngine::Viewport::onBeforeRender() {}

void GameEngine::Viewport::onAfterRender() {}

void GameEngine::Viewport::onBeforeResize(int width, int height) {}

void GameEngine::Viewport::onAfterResize(int width, int height) {}

void GameEngine::Viewport::onStatsUpdateTimerTimeout()
{
	QString projectName = "";
	if (auto project = ProjectManager::instance()->activeProject())
	{
		projectName = project->getName();
		if (auto scene = project->getActiveScene())
			projectName += " (" + scene->getName() + ")";
	}
	setTitle(projectName + " - " + RenderingManager::instance()->stats().toQString());
}
