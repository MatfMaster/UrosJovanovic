#pragma once
#include <QSet>
#include <QTimer>
#include "Includes.h"

namespace GameEngine {
	class InputManager final : public QObject
	{
		Q_OBJECT
		NOCOPY(InputManager)

	private:
		InputManager();
		~InputManager();

	public:
		bool eventFilter(QObject* object, QEvent* event) override;
		void reset();
		EXPORT static InputManager* instance();
		EXPORT bool keyPressed(int key) const;
		EXPORT int getKey() const;
		EXPORT int getKeyUp() const;
		EXPORT int getKeyDown() const;
		EXPORT int getModifiers() const;
		EXPORT int getMouseButton() const;
		EXPORT int getMouseButtons() const;
		EXPORT int getMouseButtonUp() const;
		EXPORT int getMouseButtonDown() const;
		EXPORT int getMouseX() const;
		EXPORT int getMouseY() const;
		EXPORT float getMouseXAxis() const;
		EXPORT float getMouseYAxis() const;
		EXPORT float getMouseWheelAxis() const;

	protected slots:
		void update();

	private:
		QTimer _updateTimer;
		QSet<int> _keys;
		int _key;
		int _keyUp;
		int _keyDown;
		int _modifiers;
		int _mouseButton;
		int _mouseButtons;
		int _mouseButtonUp;
		int _mouseButtonDown;
		int _mouseX;
		int _mouseY;
		int _lastMouseX;
		int _lastMouseY;
		int _mouseXAxis;
		int _mouseYAxis;
		int _lastMouseWheelAxis;
		int _mouseWheelAxis;
	};
}
