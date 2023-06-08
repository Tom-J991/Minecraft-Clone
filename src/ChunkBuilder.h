#pragma once

#include "Common.h"
#include "Math.h"

#include "Blocks.h"

class Shader;
class Texture;

class ChunkBuilder
{
public:
	ChunkBuilder(Shader *shader, Texture *texture);
	~ChunkBuilder();

	void Create();
	void CreateCube(float x, float y, float z, 
					bool xNegativeVisible, bool xPositiveVisible, 
					bool yNegativeVisible, bool yPositiveVisible, 
					bool zNegativeVisible, bool zPositiveVisible);
	void CreateMesh();

	void Update(float deltaTime);
	void Draw();

private:
	unsigned int AddVertex(glm::vec3 point, glm::vec3 normal, glm::vec2 uvCoords, float r, float g, float b, float a);
	void AddTriangle(unsigned int firstIndex, unsigned int secondIndex, unsigned int thirdIndex);

private:
	glm::vec3 m_chunkPos;

	const unsigned int m_chunkSize = 32;
	const unsigned int m_chunkHeight = 32;
	Block ***m_blocks;

	int m_bufferSize;
	unsigned int m_VAO;
	unsigned int *m_VBO;
	unsigned int m_EBO;

	int m_indicesCount = 0;
	std::vector<unsigned int> m_indices;
	std::vector<float> m_vertices;
	std::vector<float> m_normals;
	std::vector<float> m_uvCoord;
	std::vector<float> m_colors;

	glm::mat4 m_model;

	Shader *m_shader;
	Texture *m_texture;

};
