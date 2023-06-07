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
	unsigned int AddVertex(glm::vec3 point, glm::vec3 normal, glm::vec2 uvCoords);
	void AddTriangle(unsigned int firstIndex, unsigned int secondIndex, unsigned int thirdIndex);

private:
	glm::vec3 m_chunkPos;

	const unsigned int m_chunkSize = 16;
	const unsigned int m_chunkHeight = 128;
	Block ***m_blocks;

	int m_bufferSize;
	unsigned int m_VAO;
	unsigned int *m_VBO;
	unsigned int m_EBO;

	std::vector<unsigned int> m_indices;
	std::vector<float> m_vertices;
	std::vector<float> m_normals;
	std::vector<float> m_uvCoord;

	glm::mat4 m_model;

	Shader *m_shader;
	Texture *m_texture;

};
