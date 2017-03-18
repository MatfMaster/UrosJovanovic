#pragma once
#include <QTimer>
#include <QWindow>
#include <QElapsedTimer>

namespace GameEngine {
	class Viewport : public QWindow
	{
		Q_OBJECT

	public:
		virtual ~Viewport();

	protected:
		explicit Viewport(QWindow* parent = nullptr);

		bool initialized;
		QTimer statsUpdateTimer;
		QElapsedTimer updateTimer;

		/* QWindow overrides */

		bool event(QEvent* e) override;
		void exposeEvent(QExposeEvent* e) override;
		void resizeEvent(QResizeEvent* e) override;

		/* Base methods */

		void render();
		void resized(int width, int height);

		/* To be reimplemented by inheritors */

		virtual void init();
		virtual void onBeforeRender();
		virtual void onAfterRender();
		virtual void onBeforeResize(int width, int height);
		virtual void onAfterResize(int width, int height);

	private slots:
		void onStatsUpdateTimerTimeout();
	};
}
