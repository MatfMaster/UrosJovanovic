#include <QFile>
#include <QImage>
#include <QFileInfo>

#include "RenderingManagerOGL.h"
#include "RenderBufferGL.h"
#include "Rendering/Material.h"
#include "Geometry/Mesh.h"
#include "Scene/Light.h"
#include "Scene/Camera.h"
#include "GameObject.h"

/* Shaders */
#include "VertexShader.glsl"
#include "VertexShader.Skybox.glsl"
#include "FragmentShader.glsl"
#include "FragmentShader.Skybox.glsl"

GameEngine::RenderingManagerOGL::RenderingManagerOGL() {}

GameEngine::RenderingManagerOGL::~RenderingManagerOGL()
{
	for (auto vboID : _vboMap.values())
		glDeleteBuffers(1, &vboID);

	for (auto texID : _textureMap.values())
		glDeleteTextures(1, &texID);
}

void GameEngine::RenderingManagerOGL::initialize()
{
	OpenGLFuncs::initializeOpenGLFunctions();
	setOwningContext(QOpenGLContext::currentContext());

	LOG("GL_VENDOR\t" << glGetString(GL_VENDOR));
	LOG("GL_RENDERER\t" << glGetString(GL_RENDERER));
	LOG("GL_VERSION\t" << glGetString(GL_VERSION));

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glClearColor(0.0, 0.0, 0.0, 1.0);

	if (!_shader.addShaderFromSourceCode(QOpenGLShader::Vertex, vertex_shader))
		ERROR_LOG(_shader.log().toStdString());

	if (!_shader.addShaderFromSourceCode(QOpenGLShader::Fragment, fragment_shader))
		ERROR_LOG(_shader.log().toStdString());

	if (!_skyBoxShader.addShaderFromSourceCode(QOpenGLShader::Vertex, skybox_vsh))
		ERROR_LOG(_skyBoxShader.log().toStdString());

	if (!_skyBoxShader.addShaderFromSourceCode(QOpenGLShader::Fragment, skybox_fsh))
		ERROR_LOG(_skyBoxShader.log().toStdString());

	_shader.link();
	_skyBoxShader.link();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	DBG_CHECK_GL_ERRORS
}

GameEngine::RenderBuffer* GameEngine::RenderingManagerOGL::createRenderBuffer(int width, int height, RenderBufferFormat format)
{
	return new RenderBufferGL(width, height, format);
}

void GameEngine::RenderingManagerOGL::setActiveCamera(const Camera* camera)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixf(camera->projectionMatrix().constData());

	_shader.bind();
	_shader.setUniformValue("cameraPosition", camera->gameObject()->transform()->getPosition());
	_shader.release();

	DBG_CHECK_GL_ERRORS
}

void GameEngine::RenderingManagerOGL::setActiveLights(const QList<Light*>& lights)
{
	_shader.bind();
	{
		int i;
		const auto& ambientColor = Light::getAmbientLightColor();
		auto ambientColorVec = QVector3D(ambientColor.redF(), ambientColor.greenF(), ambientColor.blueF());
		for (i = 0; i < (lights.count() > 8 ? 8 : lights.count()); i++)
		{
			auto light = lights[i];
			_shader.setUniformValue(QString("lights[%1].type").arg(i).toStdString().data(), light->getType());
			_shader.setUniformValue(QString("lights[%1].position").arg(i).toStdString().data(), light->gameObject()->transform()->getPosition());
			_shader.setUniformValue(QString("lights[%1].direction").arg(i).toStdString().data(), light->gameObject()->transform()->getForward());
			_shader.setUniformValue(QString("lights[%1].color").arg(i).toStdString().data(), QVector3D(light->getColor().redF(), light->getColor().greenF(), light->getColor().blueF()));
			_shader.setUniformValue(QString("lights[%1].intensity").arg(i).toStdString().data(), light->getIntensity());
			_shader.setUniformValue(QString("lights[%1].range").arg(i).toStdString().data(), light->getRange());
			_shader.setUniformValue(QString("lights[%1].cutoff").arg(i).toStdString().data(), cosf(light->getSpotOuterAngle() * 3.14 / 180.0));
			_shader.setUniformValue(QString("lights[%1].falloff").arg(i).toStdString().data(), cosf(light->getSpotInnerAngle() * 3.14 / 180.0));
		}
		_shader.setUniformValue("lightCount", i);
		_shader.setUniformValue("ambientColor", ambientColorVec);
	}
	_shader.release();

	DBG_CHECK_GL_ERRORS
}

void GameEngine::RenderingManagerOGL::pushTransform(const QMatrix4x4& transform)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glMultMatrixf(transform.constData());

	DBG_CHECK_GL_ERRORS
}

void GameEngine::RenderingManagerOGL::popTransform(QMatrix4x4* outTransform)
{
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	DBG_CHECK_GL_ERRORS
}

void GameEngine::RenderingManagerOGL::bindMaterial(const Material& material)
{
	auto texture = material.getConstTexture();
	if (!texture.isEmpty())
	{
		GLuint tex_id;
		if (_textureMap.contains(texture.path()))
			tex_id = _textureMap[texture.path()];
		else
		{
			QFile file(texture.path());
			QFileInfo info(file);
			file.open(QFile::ReadOnly);
			auto data = file.readAll();
			file.close();

			QImage img;
			img.loadFromData(data, info.completeSuffix().toStdString().data());
			img = img.mirrored(false, true);
			img = img.convertToFormat(QImage::Format_RGBA8888);

			glGenTextures(1, &tex_id);
			glBindTexture(GL_TEXTURE_2D, tex_id);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width(), img.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img.constBits());
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);

			_textureMap.insert(texture.path(), tex_id);
		}

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex_id);
	}
	else
		glBindTexture(GL_TEXTURE_2D, 0);

	_shader.bind();
	{
		auto diffuse = material.getDiffuseColor();
		auto specular = material.getSpecularColor();
		_shader.setUniformValue("shaderType", material.getShaderType());
		_shader.setUniformValue("diffuseColor", QVector3D(diffuse.redF(), diffuse.greenF(), diffuse.blueF()));
		_shader.setUniformValue("specularColor", QVector3D(specular.redF(), specular.greenF(), specular.blueF()));
		_shader.setUniformValue("shininess", material.getShininess());
		_shader.setUniformValue("opacity", material.getOpacity());
		_shader.setUniformValue("textured", !texture.isEmpty());
		_shader.setUniformValue("textureTile", QVector2D(texture.getTileX(), texture.getTileY()));
	}
	_shader.release();

	if (material.isTwoSided())
		glDisable(GL_CULL_FACE);
	else
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}	

	DBG_CHECK_GL_ERRORS
}

void GameEngine::RenderingManagerOGL::draw(const GeometryBase* geometry)
{
	GLuint vboID = getVBO(geometry);
	if (vboID > 0)
	{
		_shader.bind();
		{
			glBindBuffer(GL_ARRAY_BUFFER, vboID);
			{
				int vertexCount = geometry->vertexCount();
				_shader.setAttributeBuffer("vertex", GL_FLOAT, 0, 3);
				_shader.setAttributeBuffer("normal", GL_FLOAT, vertexCount * 3 * sizeof(float), 3);
				_shader.setAttributeBuffer("texcoord", GL_FLOAT, 2 * vertexCount * 3 * sizeof(float), 3);
				_shader.enableAttributeArray("vertex");
				_shader.enableAttributeArray("normal");
				_shader.enableAttributeArray("texcoord");
				glDrawArrays(GL_TRIANGLES, 0, vertexCount);
				_shader.disableAttributeArray("vertex");
				_shader.disableAttributeArray("normal");
				_shader.disableAttributeArray("texcoord");
			}
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		_shader.release();

		//DEBUG NORMALS
		//glColor3f(1, 1, 1);
		//glBegin(GL_LINES);
		//{
		//	QVector3D t0, t1, t2, n0, n1, n2;
		//	for (int i = 0; i < geometry->triangleCount(); i++)
		//	{
		//		geometry->getTriangleData(i, t0, t1, t2, n0, n1, n2);

		//		glVertex3f(t0.x(), t0.y(), t0.z());
		//		glVertex3f((t0 + n0).x(), (t0 + n0).y(), (t0 + n0).z());

		//		glVertex3f(t1.x(), t1.y(), t1.z());
		//		glVertex3f((t1 + n1).x(), (t1 + n1).y(), (t1 + n1).z());

		//		glVertex3f(t2.x(), t2.y(), t2.z());
		//		glVertex3f((t2 + n2).x(), (t2 + n2).y(), (t2 + n2).z());
		//	}
		//}
		//glEnd();

		stats().currentFrame().incrementDrawCalls();
	}

	DBG_CHECK_GL_ERRORS
}

void GameEngine::RenderingManagerOGL::draw(const SkyBox* skyBox)
{
	glPushAttrib(GL_ENABLE_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthFunc(GL_LEQUAL);
	{
		GLuint cubeMapID;
		if (_skyBoxMap.contains(skyBox))
			cubeMapID = _skyBoxMap[skyBox];
		else
		{
			glGenTextures(1, &cubeMapID);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapID);

			std::vector<int> types =
				{
					GL_TEXTURE_CUBE_MAP_POSITIVE_Y, //TOP
					GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, //BOTTOM
					GL_TEXTURE_CUBE_MAP_POSITIVE_Z, //FRONT
					GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, //BACK
					GL_TEXTURE_CUBE_MAP_NEGATIVE_X, //LEFT
					GL_TEXTURE_CUBE_MAP_POSITIVE_X //RIGHT
				};

			auto textures = skyBox->textures();
			for (int i = 0; i < 6; i++)
			{
				if (textures.count() > i)
				{
					auto texture = textures[i];
					QFile file(texture.path());
					QFileInfo info(file);
					file.open(QFile::ReadOnly);
					auto data = file.readAll();
					file.close();

					QImage img;
					img.loadFromData(data, info.completeSuffix().toStdString().data());
					img = img.convertToFormat(QImage::Format_RGBA8888);
					glTexImage2D(types[i], 0, GL_RGB, img.width(), img.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img.constBits());
				}
				else
				{
					auto& color = skyBox->backgroundColor();
					const uchar data[4] = { color.red(), color.green(), color.blue(), color.alpha() };
					glTexImage2D(types[i], 0, GL_RGB, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				}
			}

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

			_skyBoxMap.insert(skyBox, cubeMapID);
		}


		glDisable(GL_CULL_FACE);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapID);
		{
			if (auto mesh = Mesh::cube())
			{
				GLuint vboID = getVBO(mesh);
				if (vboID > 0)
				{
					_skyBoxShader.bind();
					{
						glBindBuffer(GL_ARRAY_BUFFER, vboID);
						{
							_skyBoxShader.setAttributeBuffer("Position", GL_FLOAT, 0, 3);
							_skyBoxShader.enableAttributeArray("Position");
							glDrawArrays(GL_TRIANGLES, 0, mesh->vertexCount());
							_skyBoxShader.disableAttributeArray("Position");
						}
						glBindBuffer(GL_ARRAY_BUFFER, 0);
					}
					_skyBoxShader.release();

					stats().currentFrame().incrementDrawCalls();
				}
			}
		}
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
	glPopAttrib();

	DBG_CHECK_GL_ERRORS
}

void GameEngine::RenderingManagerOGL::dbgDrawLines(const Segment3D* segments, int count, const QColor& color, float thickness)
{
	glPushAttrib(GL_CURRENT_BIT | GL_LINE_BIT | GL_DEPTH_BUFFER_BIT);
	{
		glColor4f(color.redF(), color.greenF(), color.blueF(), color.alphaF());
		glLineWidth(thickness);
		glDisable(GL_DEPTH_TEST);

		glBegin(GL_LINES);
		{
			for (auto i = 0; i < count; i++)
			{
				const Segment3D& segment = segments[i];
				glVertex3f(segment.a().x(), segment.a().y(), segment.a().z());
				glVertex3f(segment.b().x(), segment.b().y(), segment.b().z());
			}
		}
		glEnd();
	}
	glPopAttrib();

	DBG_CHECK_GL_ERRORS
}

GLuint GameEngine::RenderingManagerOGL::getVBO(const GeometryBase* geometry)
{
	GLuint vboID = 0;
	if (auto mesh = dynamic_cast<const Mesh*>(geometry))
	{
		if (_vboMap.contains(mesh))
			vboID = _vboMap[mesh];
		else
		{
			// Generate VBO
			glGenBuffers(1, &vboID);
			glBindBuffer(GL_ARRAY_BUFFER, vboID);
			{
				float* bufferData = new float[3 * mesh->_verticesCount];
				std::copy(mesh->_vertices, mesh->_vertices + mesh->_verticesCount, bufferData);
				std::copy(mesh->_normals, mesh->_normals + mesh->_verticesCount, bufferData + mesh->_verticesCount);
				std::copy(mesh->_texcoords, mesh->_texcoords + mesh->_verticesCount, bufferData + 2 * mesh->_verticesCount);
				glBufferData(GL_ARRAY_BUFFER, 3 * mesh->_verticesCount * sizeof(float), bufferData, GL_STATIC_DRAW);
				delete[] bufferData;
			}
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			_vboMap.insert(mesh, vboID);
		}
	}

	DBG_CHECK_GL_ERRORS
	return vboID;
}
