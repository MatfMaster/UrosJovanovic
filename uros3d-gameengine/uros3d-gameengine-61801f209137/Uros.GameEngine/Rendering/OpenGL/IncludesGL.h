#pragma once
#include <QOpenGLFunctions_3_0>

typedef QOpenGLFunctions_3_0 OpenGLFuncs;

#ifdef _DEBUG
#define DBG_CHECK_GL_ERRORS Q_ASSERT(glGetError() == 0);
#else
#define DBG_CHECK_GL_ERRORS
#endif