#include "FixedGrid.h"

#include "Optick/include/optick.h"
#include "mmgr/mmgr.h"

FixedGrid::FixedGrid(glm::vec2 pos, glm::vec2 size, float cellSize)
{
    numColumns = (size.x / cellSize);
    numRows = (size.y / cellSize);

    this->pos = pos;
    this->size = size;
    this->cellSize = cellSize;

    int32_t numCells = numRows * numColumns;
    cell_heads.resize(numCells);
    cell_sizes.resize(numCells);
}

void FixedGrid::Clear()
{
    int32_t numCells = numRows * numColumns;
    cell_heads.reserve(numCells);
    cell_sizes.reserve(numCells);
    std::fill(cell_heads.begin(), cell_heads.end(), (uint32_t)0);
    std::fill(cell_sizes.begin(), cell_sizes.end(), (uint32_t)0);

    items_list.clear();
}

void FixedGrid::RecalculateGrid(const uint32_t& num_elements, const std::vector<bool>& static_colliders, const std::vector<glm::vec2>& positions, const std::vector<glm::vec2>& sizes)
{
    OPTICK_PUSH("Clear Grid");
    // Clear Current Grid
    Clear();
    OPTICK_POP();

    OPTICK_PUSH("First Pass");
    // First Pass (set number of items inside each cell)
    uint32_t num_items = 0;
    for (uint32_t i = 4; i < num_elements; ++i)
    {
        for (uint32_t index : GetCells(i, positions[i], sizes[i]))
        {
            cell_sizes[index]++;
            num_items++;
        }
    }
    items_list.resize(num_items); // Resize items_list to actual size (but empty)
    OPTICK_POP();

    OPTICK_PUSH("Second Pass");
    // Second Pass (set first item index of each cell)
    size_t numCells = (size_t)numRows * numColumns;
    for (uint32_t i = 1; i < numCells; ++i) // we start from second cell because the first cell will always start at head 0
    {
        cell_heads[i] = cell_heads[i - 1] + cell_sizes[i - 1];
    }
    OPTICK_POP();

    OPTICK_PUSH("Third Pass");
    // Third Pass (insert items inside items_list)
    std::vector<uint32_t> tmp_cellSizes(numCells, (uint32_t)0);
    for (uint32_t i = 4; i < num_elements; ++i)
    {
        for (uint32_t index : GetCells(i, positions[i], sizes[i]))
        {
            uint32_t item_index = cell_heads[index] + tmp_cellSizes[index];
            items_list[item_index] = i;

            tmp_cellSizes[index]++;
        }
    }
    OPTICK_POP();

}

std::vector<uint32_t> FixedGrid::GetCandidates(const uint32_t& item, const glm::vec2& pos, const glm::vec2& size) const
{
    OPTICK_PUSH("CheckCollision Candidates");

    std::vector<uint32_t> candidates = {};

    for (const uint32_t& cell_index : GetCells(item, pos, size))
    {
        const std::vector<uint32_t>::const_iterator& start = items_list.begin() + cell_heads[cell_index];
        candidates.insert(candidates.end(), start, start + cell_sizes[cell_index]);
    }

    OPTICK_POP();

    return candidates;
}

std::vector<uint32_t> FixedGrid::GetCells(const uint32_t& item, const glm::vec2& pos, const glm::vec2& size) const
{
    std::vector<uint32_t> cell_indices = {};
    int TL = -1, TR = -1, BL = -1, BR = -1;

    const int x1 = glm::floor(pos.x / cellSize);
    const int y1 = glm::floor(pos.y / cellSize);
    const int x2 = glm::floor((pos.x + size.x) / cellSize);
    const int y2 = glm::floor((pos.y + size.y) / cellSize);

    if (y1 >= 0)
    {
        // Top Left Corner
        TL = x1 + y1 * numColumns;
        if (TL < cell_heads.size() && x1 >= 0)
            cell_indices.push_back(TL);

        // Top Right Corner
        TR = x2 + y1 * numColumns;
        if (TR < cell_heads.size() && x2 >= 0 && TR != TL)
            cell_indices.push_back(TR);
    }

    if (y2 >= 0)
    {
        // Bottom Left Corner
        BL = x1 + y2 * numColumns;
        if (BL < cell_heads.size() && x1 >= 0 && BL != TL && BL != TR)
            cell_indices.push_back(BL);

        // Bottom Right Corner
        BR = x2 + y2 * numColumns;
        if (BR < cell_heads.size() && x2 >= 0 && BR != TL && BR != TR && BR != BL)
            cell_indices.push_back(BR);
    }

    // Return List
    return cell_indices;
}
