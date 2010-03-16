/*
Copyright (c) 2009-2010, Christian Reiser

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include "peakphysics/physics/Trimesh.hpp"

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionShapes/btShapeHull.h>

#include <iostream>
#include <fstream>
#include <vector>

namespace peak
{
	namespace physics
	{
		TrimeshData::TrimeshData() : vertices(0), indices(0)
		{
		}
		TrimeshData::~TrimeshData()
		{
			destroy();
		}

		bool TrimeshData::init(std::string name)
		{
			if (name.substr(name.size() - 4) == ".obj")
			{
				return initObj(name);
			}
			else if (name.substr(name.size() - 10) == ".scene.xml")
			{
				return initHorde3D(name);
			}
			else
			{
				std::cout << "Trimesh: Invalid extension: " << name << std::endl;
				return false;
			}
		}

		bool TrimeshData::init(Vector3F *vertices, int *indices, int vertexcount, int indexcount)
		{
			if (!vertices || !indices || !vertexcount || !indexcount)
				return false;

			this->vertices = vertices;
			this->indices = indices;
			this->vertexcount = vertexcount;
			this->indexcount = indexcount;
			return true;
		}
		bool TrimeshData::destroy()
		{
			if (!vertices || !indices)
				return false;
			delete[] vertices;
			vertices = 0;
			delete[] indices;
			indices = 0;
			return true;
		}

		Vector3F *TrimeshData::getVertices() const
		{
			return vertices;
		}
		int *TrimeshData::getIndices() const
		{
			return indices;
		}
		int TrimeshData::getVertexCount() const
		{
			return vertexcount;
		}
		int TrimeshData::getIndexCount() const
		{
			return indexcount;
		}

		bool TrimeshData::initObj(std::string filename)
		{
			// Open file from harddisk
			std::ifstream file(filename.c_str());
			if (!file)
				return false;

			// Declare these dynamic sized arrays because of missing vertex and index counts in .obj files
			std::vector<Vector3F> dynvertices = std::vector<Vector3F>();
			std::vector<int> dynindices = std::vector<int>();

			// Read the vertex and index data, until the virtual file end is reached.
			while (!file.eof())
			{
				std::string key;
				file >> key;
				if (key == "v")
				{
					// Vertex data
					float vertex[3] = {0.0f, 0.0f, 0.0f};
					for (unsigned int i = 0; i < 3; i++)
					{
						std::string s;
						file >> s;
						vertex[i] = (float)strtod(s.c_str(), 0);
					}
					dynvertices.push_back(Vector3F(vertex[0], vertex[1], vertex[2]));
				}
				else if (key == "f")
				{
					// Index data
					for (unsigned int i = 0; i < 3; i++)
					{
						std::string s;
						file >> s;
						dynindices.push_back(atoi(s.c_str()) - 1);
					}
				}
				else
				{
					// Skip line
					std::string line;
					std::getline(file, line);
				}
			}

			// Close file
			file.close();

			// Copy the data from dynamic sized to fixed sized arrays (better performance)
			vertexcount = dynvertices.size();
			vertices = new Vector3F[vertexcount];
			indexcount = dynindices.size();
			indices = new int[indexcount];

			for (int i = 0; i < vertexcount; i++)
				vertices[i] = dynvertices[i];
			for (int i = 0; i < indexcount; i++)
				indices[i] = dynindices[i];

			return true;
		}
		bool TrimeshData::initHorde3D(std::string filename)
		{
			// TODO
			return false;
		}

		Trimesh::Trimesh() : Shape()
		{
		}
		Trimesh::~Trimesh()
		{
		}

		bool Trimesh::init(const TrimeshData &data, float mass, bool buildhull)
		{
			// Build triangle mesh from data
			trimesh = new btTriangleMesh();
			for (int i = 0; i < data.getIndexCount() / 3; i++)
			{
				int index0 = data.getIndices()[i * 3];
				int index1 = data.getIndices()[i * 3 + 1];
				int index2 = data.getIndices()[i * 3 + 2];

				btVector3 vertex0(data.getVertices()[index0].x, data.getVertices()[index0].y, data.getVertices()[index0].z);
				btVector3 vertex1(data.getVertices()[index1].x, data.getVertices()[index1].y, data.getVertices()[index1].z);
				btVector3 vertex2(data.getVertices()[index2].x, data.getVertices()[index2].y, data.getVertices()[index2].z);

				trimesh->addTriangle(vertex0, vertex1, vertex2);
			}

			if (mass != 0.0f)
			{
				btConvexShape *convexshape = new btConvexTriangleMeshShape(trimesh);
				if (buildhull)
				{
					btShapeHull *hull = new btShapeHull(convexshape);
					btScalar margin = convexshape->getMargin();
					hull->buildHull(margin);
					convexshape->setUserPointer(hull);
					shape = new btConvexHullShape();
					btConvexHullShape *convexhullshape = (btConvexHullShape*)(shape);
					for (int j = 0; j < hull->numVertices() ; j++)
						convexhullshape->addPoint(btVector3(hull->getVertexPointer()[j]));
					delete convexshape;
					delete hull;
				}
				else
					shape = convexshape;
			}
			else
				shape = new btBvhTriangleMeshShape(trimesh, true);

			this->mass = mass;
			calculateInertia();
			return true;
		}
		bool Trimesh::destroy()
		{
			Shape::destroy();
			delete trimesh;
			return true;
		}
	}
}
