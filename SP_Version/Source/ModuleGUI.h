#pragma once
#include "Module.h"

#include <vector>

#define FPS_LOG_SIZE 100
#define PANEL_WIDTH 190
#define PANEL_HEIGHT 266
#define WORLD_SCALE 50
#define CELL_SCALE 50

struct EventWindowResize;

class ModuleGUI : public Module
{
public:
	ModuleGUI(bool start_enabled = true);
	virtual ~ModuleGUI();

	bool Init() override;
	bool Start() override;
	bool PreUpdate(float dt) override;
	bool CleanUp() override;

	void Draw();
	void DrawInfo();

	void LogFPS(float fps, float ms);

	bool IsDrawGrid() { return is_draw_grid; }
	bool IsDrawColliders() { return is_draw_colliders; }

	//--- EVENTS ---
	void OnResize(EventWindowResize* e) { is_update_pos = true; }


private:
	bool is_update_pos = false;
	bool is_draw_grid = false;
	bool is_draw_colliders = false;

	int move_speed = 0;
	int world_width = 0;
	int world_height = 0;

	int cell_size = 0;

	int num_asteroids = 0;

	std::vector<float> fps_log;
	std::vector<float> ms_log;
};
