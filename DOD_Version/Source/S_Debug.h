#pragma once
#include "System.h"

#define DEFAULT_GRID_SPACING 100

typedef unsigned int uint;

struct EventWorldSizeUpdate;

class S_Debug : public System
{
public:
    void Init();
	void Start();

    void RenderGrid();
    void RenderColliders();

	// Grid
	void CreateGrid(int width, int height, int spacing);

	const int& GetGridSpacing() const { return grid_spacing; }
	void SetGridSpacing(int spacing) { grid_spacing = spacing; UpdateGrid(grid_width, grid_height); }


private:
	void UpdateGrid(int width, int height);

	// --- EVENTS ---
	void OnWorldSizeUpdate(EventWorldSizeUpdate* e);

private:
	// Grid
	uint grid_shader = 0;
	uint gridVAO = 0;
	uint gridVBO = 0;
	int grid_vertices = 0;

	int grid_width = 0;
	int grid_height = 0;
	int grid_spacing = 0;

	// Colliders
	uint circle_texture = 0;

};
