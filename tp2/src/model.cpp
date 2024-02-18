#include "model.h"
#include "obj_loader.h"
#include <iostream>

using namespace std;

Model::Model(const char* path)
{
	//Initalisation du modèle et des attibuts de la classe
	vector<GLfloat> vertexData;
	vector<GLuint> indices;
	loadObj(path, vertexData, indices);
	m_shape.setData(vertexData.data(), vertexData.size() * sizeof(GLfloat), indices.data(), indices.size() * sizeof(GLuint));
	m_shape.enableAttribute(0, 3, 6, 0);
	m_shape.enableAttribute(1, 3, 6, 3);
	m_count = indices.size();
}

void Model::loadObj(const char* path, vector<GLfloat>& vertexData, vector<GLuint>& indices)
{
	objl::Loader loader;
	bool loadout = loader.LoadFile(path);
	if (!loadout)
	{
		cout << "Unable to load model " << path << endl;
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
