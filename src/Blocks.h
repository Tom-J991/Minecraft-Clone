#pragma once

enum class eBLOCKS
{
	NONE = 0,
	GRASS,
	DIRT,
	STONE,
	BLOCKS_MAX
};

class Block
{
public:
	Block();
	~Block();

	inline eBLOCKS GetBlockType() { return m_blockID; }
	void SetBlockType(eBLOCKS block) { m_blockID = block; }

	bool IsDrawing() { return m_draw; }
	void SetDraw(bool draw) { m_draw = draw; }

private:
	bool m_draw;

	eBLOCKS m_blockID;

};
