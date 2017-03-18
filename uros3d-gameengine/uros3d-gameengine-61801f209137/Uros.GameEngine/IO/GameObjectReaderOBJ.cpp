#include <QTime>
#include <QFile>
#include <QFileInfo>
#include <QVector>
#include "GameObject.h"
#include "GameObjectReaderOBJ.h"
#include "Geometry/Mesh.h"
#include "Rendering/MeshRenderer.h"

#define MAX_BUFFER 32

GameEngine::GameObjectReaderOBJ::GameObjectReaderOBJ(const QString& path)
	: GameObjectReader(path) {}

GameEngine::GameObject* GameEngine::GameObjectReaderOBJ::read() const
{
	auto gameObject = new GameObject();
	try
	{
		QTime stopwatch;
		stopwatch.start();

		if (!QFile::exists(path()))
            throw std::runtime_error("File not found");

		QFile file(path());
		if (!file.open(QFile::ReadOnly | QFile::Text))
            throw std::runtime_error("Cannot access file");

		QByteArray data = file.readAll();

		gameObject->setName(QFileInfo(path()).baseName());

		//Don't need it anymore
		file.close();

		QVector<QVector3D> vertices;
		QVector<QVector3D> normals;

		QString objectName;
		QVector<float> objectVertices;
		QVector<float> objectNormals;

		for (int i = 0; i < data.count(); i++)
		{
			char c = data[i];

			if (i == data.count() - 1 || c == 'v' && (i == 0 || data[i - 1] == '\n'))
			{
				if (objectVertices.count() > 0)
				{
					//Mesh from the previous object is pending

					int count = objectVertices.count();
					auto verts = new float[count];
					auto norms = new float[count];
					std::copy(objectVertices.begin(), objectVertices.end(), verts);
					std::copy(objectNormals.begin(), objectNormals.end(), norms);
					auto bbox = BoundingBox::create(verts, count);
					auto mesh = new Mesh(verts, norms, count);
					delete[] verts;
					delete[] norms;

					auto subObject = new GameObject(objectName);
					subObject->addComponent<MeshRenderer>()->setMesh(mesh);
					subObject->transform()->setParent(gameObject->transform());
					subObject->transform()->setPosition(bbox.midPoint());

					objectVertices.clear();
					objectNormals.clear();
					objectName = "";

					if (i == data.count() - 1)
						continue;
				}

				if (data[++i] == 'n')
				{
					//Vertex normal
					//vn 0.0000 0.0000 -1.0000

					i++;
					float x = readFloat(data, i);
					float y = readFloat(data, i);
					float z = readFloat(data, i);
					normals.push_back(QVector3D(x, y, z));;
				}
				else
				{
					//Vertex
					//v  -0.7500 -0.7500 0.0000

					float x = readFloat(data, i);
					float y = readFloat(data, i);
					float z = readFloat(data, i);
					vertices.push_back(QVector3D(x, y, z));
				}
			}
			else if (c == 'f' && (i == 0 || data[i - 1] == '\n'))
			{
				//Face
				//f 1//1 2//1 3//1 

				i++;
				for (int f = 0; f < 3; f++)
				{
					int v, n;
					readFace(data, i, v, n);
					objectVertices.push_back(vertices[v - 1][0]);
					objectVertices.push_back(vertices[v - 1][1]);
					objectVertices.push_back(vertices[v - 1][2]);
					objectNormals.push_back(normals[n - 1][0]);
					objectNormals.push_back(normals[n - 1][1]);
					objectNormals.push_back(normals[n - 1][2]);
				}
			}
			else if (c == 'g' && (i == 0 || data[i - 1] == '\n'))
			{
				//Group name
				//g Box001

				i += 2;
				QString name;
				while (data[i] != '\n')
				{
					name.append(static_cast<QChar>(data[i]));
					i++;
				}
				objectName = name;
			}
		}

		LOG("Loaded: " << path().toStdString() << " [" << stopwatch.elapsed() << "ms" << "]");
	}
	catch (const std::exception& ex)
	{
		//Something went wrong, log error & cleanup
		ERROR_LOG("Error reading file " << path().toStdString() << " (" << ex.what() << ")");
		GameObject::destroy(gameObject);
		gameObject = nullptr;
	}
	return gameObject;
}

float GameEngine::GameObjectReaderOBJ::readFloat(const QByteArray& data, int& idx) const
{
	char buffer[MAX_BUFFER];
	while (isspace(data[idx]))
		idx++;
	int sign = (data[idx] == '-') ? -1 : 1;
	if (sign < 0)
		idx++;
	int i = 0;
	while (!isspace(data[idx]))
		buffer[i++] = data[idx++];
	buffer[i] = '\0';
	return static_cast<float>(sign * atof(buffer));
}

void GameEngine::GameObjectReaderOBJ::readFace(const QByteArray& data, int& idx, int& v, int& n) const
{
	char buffer[MAX_BUFFER];
	while (isspace(data[idx]))
		idx++;
	int i = 0;
	while (data[idx] != '/')
		buffer[i++] = data[idx++];
	buffer[i] = '\0';
	v = atoi(buffer);
	idx++;
	while (data[idx] != '/')
		idx++;
	i = 0;
	idx++;
	while (!isspace(data[idx]))
		buffer[i++] = data[idx++];
	buffer[i] = '\0';
	n = atoi(buffer);
}
