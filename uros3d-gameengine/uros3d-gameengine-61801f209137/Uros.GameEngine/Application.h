#pragma once
#include <QCoreApplication>
#include "Includes.h"
#include "Settings.h"

namespace GameEngine {
	class Viewport;
	class Application final
	{
		NOCOPY(Application)
	private:
		Application();
		~Application();
	public:
		EXPORT static bool initialize(QCoreApplication& application, const Settings& settings);
		EXPORT static bool isRunning();
		EXPORT static void run();
		EXPORT static Settings& settings();
		EXPORT static Viewport* viewport();
	private:
		static Settings _settings;
		static Viewport* _viewport;
		static bool _running;
	};
}