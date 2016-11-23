#include "model.h"

/// <summary> Default Constructor. </summary>
Model::Model()
{
}

Model::~Model()
{
	if (m_vertexStructure != NULL)
	{
		delete m_vertexStructure;
		m_vertexStructure = NULL;

	}
	if (m_texture != NULL)
	{
		delete m_texture;
		m_texture = NULL;
	}
}

/// <summary> Load in the model, using the filepath given as param. </summary>
/// <remarks> If there is a texture available it will load in the texture as well.</remarks>
void Model::LoadModel(std::string filePath)
{
	std::ifstream fin;
	char input;

	fin.open(filePath);

	// If it could not open the file then exit.
	if (fin.fail())
	{
		return;
	}

	// Read up to the value of vertex count.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	// Read in the vertex count.
	fin >> m_vertexCount;

	// Set the number of indices to be the same as the vertex count.
	m_indexCount = m_vertexCount;

	// Create the model using the vertex count that was read in.
	m_vertexStructure = new Vertex[m_vertexCount];
	if (!m_vertexStructure)
	{
		return;
	}

	// Read up to the beginning of the data.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for (int i = 0; i< m_vertexCount; i++)
	{
		fin >> m_vertexStructure[i].coordinates.x >> m_vertexStructure[i].coordinates.y >> m_vertexStructure[i].coordinates.z;
		fin >> m_vertexStructure[i].texture.x >> m_vertexStructure[i].texture.y;
		fin >> m_vertexStructure[i].normal.x >> m_vertexStructure[i].normal.y >> m_vertexStructure[i].normal.z;
	}

	m_position = ZyphMaths::Vector3(m_vertexStructure[0].coordinates.x, m_vertexStructure[0].coordinates.y, m_vertexStructure[0].coordinates.z);
	//m_texture = new Texture("../Textures/shockwavetest.bmp", 1024, 1024);
	// Close the model file.
	fin.close();

	return;
}
