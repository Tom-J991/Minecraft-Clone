#pragma once

#include "Common.h"
#include "Math.h"

#include "ChunkBuilder.h"

class ChunkManager
{
public:
	ChunkManager();
	~ChunkManager();

private:
	std::vector<ChunkBuilder*> m_chunks;

};
