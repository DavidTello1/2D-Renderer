#pragma once
#include "Globals.h"
//#include "Event.h"

struct EventWorldSizeUpdate;

class Grid
{
public:
	Grid();
	~Grid() {};

	void Create(int width, int height, int spacing);
	void Update(int width, int height);

	void Draw();

	const int& GetGridSpacing() const { return grid_spacing; }
	void SetGridSpacing(int spacing) { grid_spacing = spacing; Update(grid_width, grid_height); }

private:
	// --- EVENTS ---
	void OnWorldSizeUpdate(EventWorldSizeUpdate* e);

private:
	uint grid_shader = 0;
	uint gridVAO = 0;
	uint gridVBO = 0;
	int grid_vertices = 0;

	int grid_width = 0;
	int grid_height = 0;
	int grid_spacing = 100;
};

