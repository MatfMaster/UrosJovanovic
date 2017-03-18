#include <QQuaternion>
#include "Mesh.h"

GameEngine::Mesh::Mesh()
	: _vertices(nullptr),
	  _normals(nullptr),
	  _texcoords(nullptr),
	  _verticesCount(0) {}

GameEngine::Mesh::Mesh(float* vertices, float* normals, int count, float* texcoords)
{
	auto boundingBox = BoundingBox::create(vertices, count);

	auto verts = new float[count];
	auto norms = new float[count];
	auto coords = new float[count];

	QVector3D center = boundingBox.midPoint();
	for (int i = 0; i < count; i += 3)
	{
		verts[i] = vertices[i] - center.x();
		verts[i + 1] = vertices[i + 1] - center.y();
		verts[i + 2] = vertices[i + 2] - center.z();
	}
	std::copy(normals, normals + count, norms);

	texcoords = texcoords ? texcoords : vertices;
	std::copy(texcoords, texcoords + count, coords);

	_vertices = verts;
	_normals = norms;
	_texcoords = coords;
	_verticesCount = count;
	_boundingBox = BoundingBox(boundingBox.minPoint() - center, boundingBox.maxPoint() - center);
}

GameEngine::Mesh::Mesh(float* vertices, float* normals, float* texcoords, int count)
{
	auto verts = new float[count];
	auto norms = new float[count];
	auto coords = new float[count];

	std::copy(vertices, vertices + count, verts);
	std::copy(normals, normals + count, norms);
	std::copy(texcoords, texcoords + count, coords);

	_vertices = verts;
	_normals = norms;
	_texcoords = coords;
	_verticesCount = count;
	_boundingBox = BoundingBox::create(_vertices, _verticesCount);
}

GameEngine::Mesh::~Mesh()
{
	delete[] _vertices;
	delete[] _normals;
	delete[] _texcoords;
}

GameEngine::Mesh* GameEngine::Mesh::cone()
{
	static Mesh* _cone;
	if (!_cone)
	{
		const int segments = 32;
		const float PI = 3.1415926;
		const float step = 360.0f / segments * (PI / 180.0f);
		float x, z;
		float r = 0.5f;
		std::vector<float> rim;
		for (int i = 0; i <= segments; i++)
		{
			x = r * cos(i * step);
			z = r * sin(i * step);
			rim.push_back(x);
			rim.push_back(0);
			rim.push_back(z);
		}
		std::vector<float> vertices;
		std::vector<float> normals;
		//Bottom cap
		for (int i = 0; i < rim.size() - 3; i += 3)
		{
			vertices.push_back(0);
			vertices.push_back(0);
			vertices.push_back(0);
			normals.push_back(0);
			normals.push_back(-1);
			normals.push_back(0);
			vertices.push_back(rim[i]);
			vertices.push_back(rim[i + 1]);
			vertices.push_back(rim[i + 2]);
			normals.push_back(0);
			normals.push_back(-1);
			normals.push_back(0);
			vertices.push_back(rim[i + 3]);
			vertices.push_back(rim[i + 4]);
			vertices.push_back(rim[i + 5]);
			normals.push_back(0);
			normals.push_back(-1);
			normals.push_back(0);
		}
		//Coat
		for (int i = 0; i < rim.size() - 3; i += 3)
		{
			float _x, _y, _z;
			QVector3D n, axis;
			QQuaternion rot;

			vertices.push_back(rim[i]);
			vertices.push_back(rim[i + 1]);
			vertices.push_back(rim[i + 2]);

			_x = rim[i];
			_y = rim[i + 1];
			_z = rim[i + 2];
			n = QVector3D(_x, _y, _z);
			axis = QVector3D::crossProduct(n, QVector3D(0, 1, 0)).normalized();
			rot = QQuaternion::fromAxisAndAngle(axis, 30);
			n = (rot * n).normalized();
			normals.push_back(n.x());
			normals.push_back(n.y());
			normals.push_back(n.z());

			vertices.push_back(rim[i + 3]);
			vertices.push_back(rim[i + 4]);
			vertices.push_back(rim[i + 5]);

			_x = rim[i + 3];
			_y = rim[i + 4];
			_z = rim[i + 5];
			n = QVector3D(_x, _y, _z);
			axis = QVector3D::crossProduct(n, QVector3D(0, 1, 0)).normalized();
			rot = QQuaternion::fromAxisAndAngle(axis, 30);
			n = (rot * n).normalized();
			normals.push_back(n.x());
			normals.push_back(n.y());
			normals.push_back(n.z());

			vertices.push_back(0);
			vertices.push_back(1);
			vertices.push_back(0);

			_x = r * cos(i / 3 * step + step / 2.0);
			_y = 0;
			_z = r * sin(i / 3 * step + step / 2.0);
			n = QVector3D(_x, _y, _z);
			axis = QVector3D::crossProduct(n, QVector3D(0, 1, 0)).normalized();
			rot = QQuaternion::fromAxisAndAngle(axis, 30);
			n = (rot * n).normalized();
			normals.push_back(n.x());
			normals.push_back(n.y());
			normals.push_back(n.z());
		}
		//TODO: texcoords
		_cone = new Mesh(&vertices[0], &normals[0], vertices.size());
	}
	return _cone;
}

GameEngine::Mesh* GameEngine::Mesh::cube()
{
	static Mesh* _cube;
	if (!_cube)
	{
		std::vector<float> vertices =
			{
				0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, /*Top*/
				0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, /*Bottom*/
				0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, /*Front*/
				1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, /*Back*/
				0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, /*Left*/
				1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1 /*Right*/
			};
		std::vector<float> normals =
			{
				0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, /*Top*/
				0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, /*Bottom*/
				0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, /*Front*/
				0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, /*Back*/
				-1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, -1, 0, 0, /*Left*/
				1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0 /*Right*/
			};
		std::vector<float> texcoords =
			{
				0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0,
				0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0,
				0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0,
				0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0,
				0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0,
				0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0
			};
		_cube = new Mesh(&vertices[0], &normals[0], vertices.size(), &texcoords[0]);
	}
	return _cube;
}

GameEngine::Mesh* GameEngine::Mesh::plane()
{
	static Mesh* _plane;
	if (!_plane)
	{
		std::vector<float> vertices = { 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0 };
		std::vector<float> normals = { 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0 };
		std::vector<float> texcoords = { 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0 };
		_plane = new Mesh(&vertices[0], &normals[0], vertices.size(), &texcoords[0]);
	}
	return _plane;
}

GameEngine::Mesh* GameEngine::Mesh::sphere()
{
	const float PI = 3.1415926;

	static Mesh* _sphere;
	if (!_sphere)
	{
		const int segments = 32;
		const float step = 360.0f / segments * (PI / 180.0f);
		float r, x, y, z;
		std::vector<float> ring;
		std::vector<std::vector<float>> rings;
		std::vector<float> vertices;
		//Generate sphere vertices one ring at a time
		for (int i = segments / 4; i <= segments / 4 * 3; i++)
		{
			ring.clear();
			r = 0.5 * cos(i * step);
			y = 0.5 * sin(i * step);
			int start = segments / 4;
			int end = 5 * segments / 4 - 1;
			for (int j = start; j <= end; j++)
			{
				x = r * cos(j * step);
				z = r * sin(j * step);
				ring.push_back(x);
				ring.push_back(y);
				ring.push_back(z);
			}
			x = r * cos(end * step + step * 0.999998); //The mesh isn't quite closed, because of texture seams bug
			z = r * sin(end * step + step * 0.999998);
			ring.push_back(x);
			ring.push_back(y);
			ring.push_back(z);
			if (i > segments / 4)
			{
				const std::vector<float>& prev = rings.back();
				for (int j = 0; j < ring.size() - 3; j += 3)
				{
					if (i < segments / 4 * 3)
					{
						vertices.push_back(ring[j]);
						vertices.push_back(ring[j + 1]);
						vertices.push_back(ring[j + 2]);

						vertices.push_back(prev[j]);
						vertices.push_back(prev[j + 1]);
						vertices.push_back(prev[j + 2]);

						vertices.push_back(ring[j + 3]);
						vertices.push_back(ring[j + 4]);
						vertices.push_back(ring[j + 5]);
					}
					if (i > segments / 4 + 1)
					{
						vertices.push_back(prev[j + 3]);
						vertices.push_back(prev[j + 4]);
						vertices.push_back(prev[j + 5]);

						vertices.push_back(ring[j + 3]);
						vertices.push_back(ring[j + 4]);
						vertices.push_back(ring[j + 5]);

						vertices.push_back(prev[j]);
						vertices.push_back(prev[j + 1]);
						vertices.push_back(prev[j + 2]);
					}
				}
			}
			rings.push_back(ring);
		}

		std::vector<float> texcoords;
		texcoords.reserve(vertices.size());
		for (int i = 0; i < vertices.size(); i += 3)
		{
			QVector3D vec(vertices[i], vertices[i + 1], vertices[i + 2]);
			vec.normalize();

			float u = (atan2(vec.x(), vec.z()) / PI + 1.0) * 0.5;
			float v = asin(vec.y()) / PI + 0.5;

			texcoords.push_back(u);
			texcoords.push_back(v);
			texcoords.push_back(0);
		}

		_sphere = new Mesh(&vertices[0], &vertices[0], vertices.size(), &texcoords[0]);
	}
	return _sphere;
}

GameEngine::Mesh* GameEngine::Mesh::cylinder()
{
	static Mesh* _cylinder;
	if (!_cylinder)
	{
		const int segments = 32;
		const float PI = 3.1415926;
		const float step = 360.0f / segments * (PI / 180.0f);
		float x, z;
		float r = 0.5f;
		std::vector<float> rim;
		for (int i = 0; i <= segments; i++)
		{
			x = r * cos(i * step);
			z = r * sin(i * step);
			rim.push_back(x);
			rim.push_back(0);
			rim.push_back(z);
		}
		std::vector<float> verts;
		std::vector<float> norms;
		//Bottom cap
		for (int i = 0; i < rim.size() - 3; i += 3)
		{
			verts.push_back(0);
			verts.push_back(0);
			verts.push_back(0);
			norms.push_back(0);
			norms.push_back(-1);
			norms.push_back(0);
			verts.push_back(rim[i]);
			verts.push_back(rim[i + 1]);
			verts.push_back(rim[i + 2]);
			norms.push_back(0);
			norms.push_back(-1);
			norms.push_back(0);
			verts.push_back(rim[i + 3]);
			verts.push_back(rim[i + 4]);
			verts.push_back(rim[i + 5]);
			norms.push_back(0);
			norms.push_back(-1);
			norms.push_back(0);
		}
		//Coat
		for (int i = 0; i < rim.size() - 3; i += 3)
		{
			verts.push_back(rim[i]);
			verts.push_back(rim[i + 1]);
			verts.push_back(rim[i + 2]);
			norms.push_back(rim[i] * 2);
			norms.push_back(rim[i + 1] * 2);
			norms.push_back(rim[i + 2] * 2);

			verts.push_back(rim[i + 3]);
			verts.push_back(rim[i + 4]);
			verts.push_back(rim[i + 5]);
			norms.push_back(rim[i + 3] * 2);
			norms.push_back(rim[i + 4] * 2);
			norms.push_back(rim[i + 5] * 2);

			verts.push_back(rim[i + 3]);
			verts.push_back(rim[i + 4] + 1);
			verts.push_back(rim[i + 5]);
			norms.push_back(rim[i + 3] * 2);
			norms.push_back(rim[i + 4] * 2);
			norms.push_back(rim[i + 5] * 2);

			verts.push_back(rim[i + 3]);
			verts.push_back(rim[i + 4] + 1);
			verts.push_back(rim[i + 5]);
			norms.push_back(rim[i + 3] * 2);
			norms.push_back(rim[i + 4] * 2);
			norms.push_back(rim[i + 5] * 2);

			verts.push_back(rim[i]);
			verts.push_back(rim[i + 1] + 1);
			verts.push_back(rim[i + 2]);
			norms.push_back(rim[i] * 2);
			norms.push_back(rim[i + 1] * 2);
			norms.push_back(rim[i + 2] * 2);

			verts.push_back(rim[i]);
			verts.push_back(rim[i + 1]);
			verts.push_back(rim[i + 2]);
			norms.push_back(rim[i] * 2);
			norms.push_back(rim[i + 1] * 2);
			norms.push_back(rim[i + 2] * 2);
		}
		//Top cap
		for (int i = 0; i < rim.size() - 3; i += 3)
		{
			verts.push_back(0);
			verts.push_back(1);
			verts.push_back(0);
			norms.push_back(0);
			norms.push_back(1);
			norms.push_back(0);
			verts.push_back(rim[i]);
			verts.push_back(rim[i + 1] + 1);
			verts.push_back(rim[i + 2]);
			norms.push_back(0);
			norms.push_back(1);
			norms.push_back(0);
			verts.push_back(rim[i + 3]);
			verts.push_back(rim[i + 4] + 1);
			verts.push_back(rim[i + 5]);
			norms.push_back(0);
			norms.push_back(1);
			norms.push_back(0);
		}
		//TODO: texcoords
		_cylinder = new Mesh(&verts[0], &norms[0], verts.size());
	}
	return _cylinder;
}

GameEngine::GeometryBase* GameEngine::Mesh::clone() const
{
	auto mesh = new Mesh();
	mesh->_vertices = new float[_verticesCount];
	mesh->_normals = new float[_verticesCount];
	mesh->_texcoords = new float[_verticesCount];
	mesh->_verticesCount = _verticesCount;
	mesh->_boundingBox = _boundingBox;
	std::copy(_vertices, _vertices + _verticesCount, mesh->_vertices);
	std::copy(_normals, _normals + _verticesCount, mesh->_normals);
	std::copy(_texcoords, _texcoords + _verticesCount, mesh->_texcoords);
	return mesh;
}

int GameEngine::Mesh::vertexCount() const
{
	return _verticesCount / 3;
}

int GameEngine::Mesh::triangleCount() const
{
	return _verticesCount / 9;
}

void GameEngine::Mesh::getVertexData(int index, QVector3D& v, QVector3D& n) const
{
	Q_ASSERT(index >= 0 && index < vertexCount());

	v = QVector3D(_vertices[index * 3], _vertices[index * 3 + 1], _vertices[index * 3 + 2]);
	n = QVector3D(_normals[index * 3], _normals[index * 3 + 1], _normals[index * 3 + 2]);
}

void GameEngine::Mesh::getTriangleData(int index, QVector3D& t0, QVector3D& t1, QVector3D& t2, QVector3D& n0, QVector3D& n1, QVector3D& n2) const
{
	Q_ASSERT(index >= 0 && index < triangleCount());

	getVertexData(index * 3, t0, n0);
	getVertexData(index * 3 + 1, t1, n1);
	getVertexData(index * 3 + 2, t2, n2);
}

const GameEngine::BoundingBox& GameEngine::Mesh::boundingBox() const
{
	return _boundingBox;
}

void GameEngine::Mesh::getTextureCoord(int index, QVector3D& coord)
{
	Q_ASSERT(index >= 0 && index < vertexCount());

	coord = QVector3D(_texcoords[index * 3], _texcoords[index * 3 + 1], _texcoords[index * 3 + 2]);
}
