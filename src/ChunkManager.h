#pragma once

#include "Common.h"
#include "Math.h"

#include "ChunkBuilder.h"

struct Camera;

class ChunkManager
{
public:
	ChunkManager();
	~ChunkManager();

	void Update(float deltaTime, Camera& camera);

private:

private:
	glm::vec3 m_cameraPos;
	glm::mat4 m_cameraView;

};
