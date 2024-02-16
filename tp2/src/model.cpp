#include "model.h"
#include "obj_loader.h"
#include <iostream>

using namespace std;

Model::Model(const char* path) : m_shape(nullptr, 0, nullptr, 0), m_count(0)
{
    vector<GLfloat> vertexData;
    vector<GLuint> indices;

    loadObj(path, vertexData, indices);

    GLfloat* data = vertexData.data();
    GLuint* indexes = indices.data();

    GLsizeiptr byteSize = vertexData.size() * sizeof(GLfloat);
    GLsizeiptr indexesByteSize = indices.size() * sizeof(GLuint);

    m_shape = BasicShapeElements(data, byteSize, indexes, indexesByteSize);
    m_count = indices.size();
}


void Model::loadObj(const char* path, std::vector<GLfloat>& vertexData, std::vector<GLuint>& indices)
{
	objl::Loader loader;
	bool loadout = loader.LoadFile(path);
	if (!loadout)
	{
		std::cout << "Unable to load model " << path << std::endl;
		return;
	}

	for (size_t i = 0; i < loader.LoadedVertices.size(); i++)
	{
		objl::Vector3 p = loader.LoadedVertices[i].Position;
		vertexData.push_back(p.X);
		vertexData.push_back(p.Y);
		vertexData.push_back(p.Z);
		// TODO: Décommenter lors de la partie 2
		//objl::Vector2 t = loader.LoadedVertices[i].TextureCoordinate;
		//vertexData.push_back(t.X);
		//vertexData.push_back(t.Y);
	}
	indices = loader.LoadedIndices;
}

void Model::draw()
{
	// Dessine le modèle en triangle
	m_shape.draw(GL_TRIANGLES, m_count);
}
