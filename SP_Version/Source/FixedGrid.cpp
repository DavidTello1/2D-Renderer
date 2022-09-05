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
    cell_heads.reserve(numCells);
    cell_sizes.reserve(numCells);

    for (size_t i = 0; i < numCells; ++i)
    {
        cell_heads.push_back((uint32_t) 0);
        cell_sizes.push_back((uint32_t) 0);
    }
}

void FixedGrid::Clear()
{
    int32_t numCells = numRows * numColumns;
    for (size_t i = 0; i < numCells; ++i)
    {
        cell_heads[i] = (uint32_t)0;
        cell_sizes[i] = (uint32_t)0;
    }

    items_list.clear();
}

void FixedGrid::Insert(const uint32_t& item, const glm::vec2& pos, const glm::vec2& size)
{
    // For each cell occupied by the item
    for (uint32_t cell_index : GetCells(item, pos, size))
    {
        // Insert item to list after last-item's index inside this cell
        items_list.insert(items_list.begin() + cell_heads[cell_index] + cell_sizes[cell_index], item);
        
        // Increase count of cell's items
        cell_sizes[cell_index]++;

        // Update all cell_heads behind this cell
        for (size_t i = cell_index + 1; i < cell_heads.size(); ++i)
            cell_heads[i]++;
    }
}

void FixedGrid::Remove(const uint32_t& item, const glm::vec2& pos, const glm::vec2& size)
{
    // For each cell occupied by the item
    for (uint32_t cell_index : GetCells(item, pos, size))
    {
        for (size_t i = 0; i < cell_sizes[cell_index]; ++i) // For each item inside the cell
        {
            uint index = cell_heads[cell_index] + i; // Get Index to items_list

            if (items_list[index] == item) // If found
            {
                items_list.erase(items_list.begin() + index); // Erase the item from the items_list
                break;
            }
        }

        // Decrease count of cell's items
        cell_sizes[cell_index]--; 

        // Update all cell_heads behind this cell
        for (size_t i = cell_index + 1; i < cell_heads.size(); ++i)
            cell_heads[i]--;
    }
}

std::vector<uint32_t> FixedGrid::GetCandidates(const uint32_t& item, const glm::vec2& pos, const glm::vec2& size) const
{
    OPTICK_PUSH("CheckCollision Candidates");

    std::vector<uint32_t> candidates = {};

    for (const uint32_t& cell_index : GetCells(item, pos, size))
    {
        OPTICK_PUSH("GetCells");
        const uint32_t cell_size = cell_sizes[cell_index];
        const uint32_t cell_head = cell_heads[cell_index];
        for (uint32_t i = 0; i < cell_size; ++i)
        {
            uint32_t item_index = cell_head + i;
            candidates.push_back(items_list[item_index]);
        }
        OPTICK_POP();
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
