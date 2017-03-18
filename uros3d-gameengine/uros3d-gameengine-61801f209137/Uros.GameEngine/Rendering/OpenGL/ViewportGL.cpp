#include "ViewportGL.h"

GameEngine::ViewportGL::ViewportGL(QWindow* parent)
	: Viewport(parent),
	  _context(nullptr)
{
	setSurfaceType(OpenGLSurface);
}

GameEngine::ViewportGL::~ViewportGL()
{
	delete _context;
}

void GameEngine::ViewportGL::init()
{
	if (!_context)
	{
		// Create context
		_context = new QOpenGLContext(this);
		_context->setFormat(requestedFormat());
		_context->create();
		_context->makeCurrent(this);

		// Init GL stuff
		initializeOpenGLFunctions();
	}
}

void GameEngine::ViewportGL::onBeforeRender()
{
	// Activate context
	_context->makeCurrent(this);

	// Clear screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


#ifdef _DEBUG

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/* Draw gridlines */
	glLineWidth(1.0f);
	glColor3f(0.3, 0.3, 0.3);
	for (int i = -5; i <= 5; i++)
	{
		glBegin(GL_LINES);
		glVertex3f(i, 0.001, -5);
		glVertex3f(i, 0.001, 5);
		glEnd();

		glBegin(GL_LINES);
		glVertex3f(-5, 0.001, i);
		glVertex3f(5, 0.001, i);
		glEnd();
	}

	/* Draw XYZ axes */
	glLineWidth(3.0f);

	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(1, 0, 0);
	glEnd();

	glColor3f(0, 1, 0);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 1, 0);
	glEnd();

	glColor3f(0, 0, 1);
	glBegin(GL_LINES);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, -1);
	glEnd();

#endif
}

void GameEngine::ViewportGL::onAfterRender()
{
	// Push to screen
	_context->swapBuffers(this);
}

void GameEngine::ViewportGL::onAfterResize(int width, int height)
{
	// Setup GL stuff
	if (_context)
		glViewport(0, 0, width, height);
}
