#ifndef _MODEL_H_
#define _MODEL_H_

#include "vector3.h"
#include "vector2.h"
#include "texture.h" 
#include <fstream>

class Model
{
public:
	struct Vertex
	{
		ZyphMaths::Vector3 coordinates;
		ZyphMaths::Vector2 texture;
		ZyphMaths::Vector3 worldCoordinates;
		ZyphMaths::Vector3 normal;
	};

	Vertex* m_vertexStructure;
	Texture* m_texture;
	ZyphMaths::Vector3 m_position;
	ZyphMaths::Vector3 m_rotation;
	int m_vertexCount;
	int m_indexCount;

	Model();
	virtual ~Model();
	void LoadModel(std::string filePath);
};
#endif