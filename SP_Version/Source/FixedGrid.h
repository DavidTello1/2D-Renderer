#pragma once
#include "Globals.h"

#include "glm/include/glm/glm.hpp"
#include <vector>

class FixedGrid
{
public:
	FixedGrid(glm::vec2 pos, glm::vec2 size, float cellSize);
	virtual ~FixedGrid() {};

	const glm::vec2& GetPosition() const { return pos; }
	const glm::vec2& GetSize() const { return size; }
	const uint& GetNumRows() const { return numRows; }
	const uint& GetNumColumns() const { return numColumns; }
	const float& GetCellSize() const { return cellSize; }
	void SetCellSize(const float& size) { cellSize = size; }

	void Clear();
	void RecalculateGrid(const uint32_t& num_elements, const std::vector<bool>& static_colliders, const std::vector<glm::vec2>& positions, const std::vector<glm::vec2>& sizes);

	std::vector<uint32_t> GetCandidates(const uint32_t& item, const glm::vec2& pos, const glm::vec2& size) const;

private:
	std::vector<uint32_t> GetCells(const uint32_t& item, const glm::vec2& pos, const glm::vec2& size) const;

private:
	uint numRows = 0;
	uint numColumns = 0;

	glm::vec2 pos = glm::vec2(0.0f);
	glm::vec2 size = glm::vec2(0.0f);
	float cellSize = 0.0f;

	// Grid Cells
	std::vector<uint32_t> cell_heads; //index of first item in cell inside items_list
	std::vector<uint32_t> cell_sizes; //number of items inside each cell

	// Items
	std::vector<uint32_t> items_list;
};
