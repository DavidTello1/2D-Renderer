#pragma once
#include "Module.h"

#define DEFAULT_GRID_SPACING 100

struct EventWorldSizeUpdate;

class ModuleDebug : public Module
{
public:
	ModuleDebug(bool start_enabled = true);
	virtual ~ModuleDebug();

	bool Init() override;
	bool Start() override;
	bool Update(float dt) override;
	bool CleanUp() override;

	void DrawColliders();
	void DrawGrid();

	// Colliders
	bool IsDrawColliders() { return is_draw_colliders; }
	void SetDrawColliders(bool value) { is_draw_colliders = value; }

	// Grid
	bool IsDrawGrid() { return is_draw_grid; }
	void SetDrawGrid(bool value) { is_draw_grid = value; }
	const int& GetGridSpacing() const { return grid_spacing; }
	void SetGridSpacing(int spacing) { grid_spacing = spacing; UpdateGrid(grid_width, grid_height); }

private:
	void CreateGrid(int width, int height, int spacing);
	void UpdateGrid(int width, int height);

	// --- EVENTS ---
	void OnWorldSizeUpdate(EventWorldSizeUpdate* e);

private:
	bool is_draw_grid = false;
	bool is_draw_colliders = false;

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

