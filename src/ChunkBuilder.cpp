#include "ChunkBuilder.h"

#include "Shader.h"
#include "Texture.h"

#include <glad/glad.h>

#define FNL_IMPL
#include <FastNoiseLite.h>

ChunkBuilder::ChunkBuilder(Shader *shader, Texture *texture)
	: m_shader { shader }
	, m_texture { texture }
{ }
ChunkBuilder::~ChunkBuilder()
{ 
	for (int i = 0; i < m_chunkSize; ++i)
	{
		for (int j = 0; j < m_chunkHeight; ++j)
		{
			delete[] m_blocks[i][j];
		}
		delete[] m_blocks[i];
	}
	delete[] m_blocks;

	spdlog::info("Destroyed Chunk.");
}

void ChunkBuilder::Create()
{
	spdlog::info("Creating Chunk.");

	m_blocks = new Block**[m_chunkSize];
	for (int i = 0; i < m_chunkSize; i++)
	{
		m_blocks[i] = new Block*[m_chunkHeight];
		for (int j = 0; j < m_chunkHeight; j++)
		{
			m_blocks[i][j] = new Block[m_chunkSize];
		}
	}

	// Terrain
	fnl_state noise = fnlCreateState();
	noise.noise_type = FNL_NOISE_OPENSIMPLEX2;
	for (int x = 0; x < m_chunkSize; x++)
	{
		for (int y = 0; y < m_chunkHeight; y++)
		{
			for (int z = 0; z < m_chunkSize; z++)
			{
				const float noiseScale = 0.1f;
				float noiseMap = fnlGetNoise2D(&noise, x*noiseScale, z*noiseScale);
				float height = (m_chunkHeight * 0.5f) * noiseMap;

				m_blocks[x][y][z].SetDraw(true);
				if (y <= m_chunkHeight * 0.5f + height)
				{
					m_blocks[x][y][z].SetBlockType(eBLOCKS::DIRT);
					if (y > (m_chunkHeight * 0.5f + height) - 1)
						m_blocks[x][y][z].SetBlockType(eBLOCKS::GRASS);
					if (y < (m_chunkHeight * 0.5f + height) - 4)
						m_blocks[x][y][z].SetBlockType(eBLOCKS::STONE);
				}
				else
				{
					m_blocks[x][y][z].SetDraw(false);
					m_blocks[x][y][z].SetBlockType(eBLOCKS::NONE);
				}
			}
		}
	}
}

void ChunkBuilder::CreateCube(float x, float y, float z,
							bool xNegativeVisible, bool xPositiveVisible,
							bool yNegativeVisible, bool yPositiveVisible,
							bool zNegativeVisible, bool zPositiveVisible)
{
	// Vertex Points
	const float blockSize = 1.0f;
	float xPos = x * 2.0f;
	float yPos = y * 2.0f;
	float zPos = z * 2.0f;
	glm::vec3 p1 = { xPos - blockSize, yPos - blockSize, zPos + blockSize };
	glm::vec3 p2 = { xPos + blockSize, yPos - blockSize, zPos + blockSize };
	glm::vec3 p3 = { xPos + blockSize, yPos + blockSize, zPos + blockSize };
	glm::vec3 p4 = { xPos - blockSize, yPos + blockSize, zPos + blockSize };
	glm::vec3 p5 = { xPos + blockSize, yPos - blockSize, zPos - blockSize };
	glm::vec3 p6 = { xPos - blockSize, yPos - blockSize, zPos - blockSize };
	glm::vec3 p7 = { xPos - blockSize, yPos + blockSize, zPos - blockSize };
	glm::vec3 p8 = { xPos + blockSize, yPos + blockSize, zPos - blockSize };

	// Set Block Texture Sides
	eBLOCKS blockType = m_blocks[(int)x][(int)y][(int)z].GetBlockType();
	int frontIndex, backIndex, rightIndex, leftIndex, topIndex, bottomIndex;
	switch (blockType)
	{
		case eBLOCKS::GRASS:
		{
			frontIndex		= 2;
			backIndex		= 2;
			rightIndex		= 2;
			leftIndex		= 2;
			topIndex		= 0;
			bottomIndex		= 1;
		} break;
		case eBLOCKS::DIRT:
		{
			frontIndex		= 1;
			backIndex		= 1;
			rightIndex		= 1;
			leftIndex		= 1;
			topIndex		= 1;
			bottomIndex		= 1;
		} break;
		case eBLOCKS::STONE:
		{
			frontIndex		= 3;
			backIndex		= 3;
			rightIndex		= 3;
			leftIndex		= 3;
			topIndex		= 3;
			bottomIndex		= 3;
		} break;
		default:
		{
			frontIndex		= 0;
			backIndex		= 0;
			rightIndex		= 0;
			leftIndex		= 0;
			topIndex		= 0;
			bottomIndex		= 0;
		} break;
	}

	// UV Coordinates
	const float uvOff = 1.0f/8.0f;
	glm::vec2 uv1, uv2, uv3, uv4;

	glm::vec3 n1; // Normals

	// Create Mesh
	unsigned int v1, v2, v3, v4, v5, v6, v7, v8;

	// Front
	if (zPositiveVisible == false)
	{
		n1 = { 0.0f, 0.0f, 1.0f };
		uv1 = { (float)(frontIndex%8)/8, uvOff+(float)(frontIndex/8)/8 };
		uv2 = { uvOff+(float)(frontIndex%8)/8, uvOff+(float)(frontIndex/8)/8 };
		uv3 = { uvOff+(float)(frontIndex%8)/8, (float)(frontIndex/8)/8 };
		uv4 = { (float)(frontIndex%8)/8, (float)(frontIndex/8)/8 };
		v1 = AddVertex(p1, n1, uv1);
		v2 = AddVertex(p2, n1, uv2);
		v3 = AddVertex(p3, n1, uv3);
		v4 = AddVertex(p4, n1, uv4);
		AddTriangle(v1, v2, v3); // Tri 1
		AddTriangle(v1, v3, v4); // Tri 2
	}
	// Back
	if (zNegativeVisible == false)
	{
		n1 = { 0.0f, 0.0f, -1.0f };
		uv1 = { (float)(backIndex%8)/8, uvOff+(float)(backIndex/8)/8 };
		uv2 = { uvOff+(float)(backIndex%8)/8, uvOff+(float)(backIndex/8)/8 };
		uv3 = { uvOff+(float)(backIndex%8)/8, (float)(backIndex/8)/8 };
		uv4 = { (float)(backIndex%8)/8, (float)(backIndex/8)/8 };
		v5 = AddVertex(p5, n1, uv1);
		v6 = AddVertex(p6, n1, uv2);
		v7 = AddVertex(p7, n1, uv3);
		v8 = AddVertex(p8, n1, uv4);
		AddTriangle(v5, v6, v7); // Tri 1
		AddTriangle(v5, v7, v8); // Tri 2
	}
	// Right
	if (xPositiveVisible == false)
	{
		n1 ={ 1.0f, 0.0f, 0.0f };
		uv1 ={ (float)(rightIndex%8)/8, uvOff+(float)(rightIndex/8)/8 };
		uv2 ={ uvOff+(float)(rightIndex%8)/8, uvOff+(float)(rightIndex/8)/8 };
		uv3 ={ uvOff+(float)(rightIndex%8)/8, (float)(rightIndex/8)/8 };
		uv4 ={ (float)(rightIndex%8)/8, (float)(rightIndex/8)/8 };
		v2 = AddVertex(p2, n1, uv1);
		v5 = AddVertex(p5, n1, uv2);
		v8 = AddVertex(p8, n1, uv3);
		v3 = AddVertex(p3, n1, uv4);
		AddTriangle(v2, v5, v8); // Tri 1
		AddTriangle(v2, v8, v3); // Tri 2
	}
	// Left
	if (xNegativeVisible == false)
	{
		n1 ={ -1.0f, 0.0f, 0.0f };
		uv1 ={ (float)(leftIndex%8)/8, uvOff+(float)(leftIndex/8)/8 };
		uv2 ={ uvOff+(float)(leftIndex%8)/8, uvOff+(float)(leftIndex/8)/8 };
		uv3 ={ uvOff+(float)(leftIndex%8)/8, (float)(leftIndex/8)/8 };
		uv4 ={ (float)(leftIndex%8)/8, (float)(leftIndex/8)/8 };
		v6 = AddVertex(p6, n1, uv1);
		v1 = AddVertex(p1, n1, uv2);
		v4 = AddVertex(p4, n1, uv3);
		v7 = AddVertex(p7, n1, uv4);
		AddTriangle(v6, v1, v4); // Tri 1
		AddTriangle(v6, v4, v7); // Tri 2
	}
	// Top
	if (yPositiveVisible == false)
	{
		n1 ={ 0.0f, 1.0f, 0.0f };
		uv1 ={ (float)(topIndex%8)/8, uvOff+(float)(topIndex/8)/8 };
		uv2 ={ uvOff+(float)(topIndex%8)/8, uvOff+(float)(topIndex/8)/8 };
		uv3 ={ uvOff+(float)(topIndex%8)/8, (float)(topIndex/8)/8 };
		uv4 ={ (float)(topIndex%8)/8, (float)(topIndex/8)/8 };
		v4 = AddVertex(p4, n1, uv1);
		v3 = AddVertex(p3, n1, uv2);
		v8 = AddVertex(p8, n1, uv3);
		v7 = AddVertex(p7, n1, uv4);
		AddTriangle(v4, v3, v8); // Tri 1
		AddTriangle(v4, v8, v7); // Tri 2
	}
	// Bottom
	if (yNegativeVisible == false)
	{
		n1 ={ 0.0f, -1.0f, 0.0f };
		uv1 ={ (float)(bottomIndex%8)/8, uvOff+(float)(bottomIndex/8)/8 };
		uv2 ={ uvOff+(float)(bottomIndex%8)/8, uvOff+(float)(bottomIndex/8)/8 };
		uv3 ={ uvOff+(float)(bottomIndex%8)/8, (float)(bottomIndex/8)/8 };
		uv4 ={ (float)(bottomIndex%8)/8, (float)(bottomIndex/8)/8 };
		v6 = AddVertex(p6, n1, uv1);
		v5 = AddVertex(p5, n1, uv2);
		v2 = AddVertex(p2, n1, uv3);
		v1 = AddVertex(p1, n1, uv4);
		AddTriangle(v6, v5, v2); // Tri 1
		AddTriangle(v6, v2, v1); // Tri 2
	}

}
void ChunkBuilder::CreateMesh()
{
	spdlog::info("Creating Chunk Mesh.");

	for (int x = 0; x < m_chunkSize; x++)
	{
		for (int y = 0; y < m_chunkHeight; y++)
		{
			for (int z = 0; z < m_chunkSize; z++)
			{
				// Skip if not drawing block
				if (m_blocks[x][y][z].IsDrawing() == false)
					continue;
				// Check Neighbors
				bool nXNegative = false;
				if (x > 0)
					nXNegative = m_blocks[x-1][y][z].IsDrawing();
				bool nXPositive = false;
				if (x < m_chunkSize-1)
					nXPositive = m_blocks[x+1][y][z].IsDrawing();
				bool nYNegative = false;
				if (y > 0)
					nYNegative = m_blocks[x][y-1][z].IsDrawing();
				bool nYPositive = false;
				if (y < m_chunkHeight-1)
					nYPositive = m_blocks[x][y+1][z].IsDrawing();
				bool nZNegative = false;
				if (z > 0)
					nZNegative = m_blocks[x][y][z-1].IsDrawing();
				bool nZPositive = false;
				if (z < m_chunkSize-1)
					nZPositive = m_blocks[x][y][z+1].IsDrawing();
				// Create
				CreateCube(x, y, z, nXNegative, nXPositive, nYNegative, nYPositive, nZNegative, nZPositive);
			}
		}
	}

	m_bufferSize = 3;
	glGenVertexArrays(1, &m_VAO);
	m_VBO = new unsigned int[m_bufferSize];
	glGenBuffers(m_bufferSize, m_VBO);

	glBindVertexArray(m_VAO);

	// Vertices
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(float), &m_vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	// Normals
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, m_normals.size() * sizeof(float), &m_normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(1);
	// UV Coordinates
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, m_uvCoord.size() * sizeof(float), &m_uvCoord[0], GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void ChunkBuilder::Update(float deltaTime)
{
}

void ChunkBuilder::Draw()
{
	m_model = glm::mat4(1.0f);
	m_model = glm::translate(m_model, m_chunkPos);

	m_shader->Use();
	m_shader->SetMatrix4("model", m_model);
	glActiveTexture(GL_TEXTURE0);
	m_texture->Use();
	m_shader->SetInteger("mainTexture", 0);

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, (GLsizei)m_indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

unsigned int ChunkBuilder::AddVertex(glm::vec3 point, glm::vec3 normal, glm::vec2 uvCoords)
{
	static int index = -1;
	index++;

	m_vertices.push_back(point.x);
	m_vertices.push_back(point.y);
	m_vertices.push_back(point.z);

	m_normals.push_back(normal.x);
	m_normals.push_back(normal.y);
	m_normals.push_back(normal.z);

	m_uvCoord.push_back(uvCoords.x);
	m_uvCoord.push_back(uvCoords.y);

	return index;
}
void ChunkBuilder::AddTriangle(unsigned int firstIndex, unsigned int secondIndex, unsigned int thirdIndex)
{
	m_indices.push_back(firstIndex);
	m_indices.push_back(secondIndex);
	m_indices.push_back(thirdIndex);
}
