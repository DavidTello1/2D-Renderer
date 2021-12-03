#pragma once
#include "Module.h"

#define FPS_LOG_SIZE 100
#define PANEL_WIDTH 186
#define PANEL_HEIGHT 266
#define WORLD_SCALE 50

class ModuleGUI : public Module
{
public:
	ModuleGUI(bool start_enabled = true);
	virtual ~ModuleGUI();

	bool Init() override;
	bool Start() override;
	bool PreUpdate(float dt) override;
	bool Update(float dt) override;
	bool PostUpdate(float dt) override;
	bool CleanUp() override;

	void Draw();
	void DrawInfo();

	void LogFPS(float fps, float ms);

public:
	bool is_update_pos = false;
	bool is_debug = false;

private:
	int num_asteroids = 0;
	int seed = 0;

	int move_speed = 0;
	float zoom_speed = 0.0f;

	int world_width = 0;
	int world_height = 0;

	std::vector<float> fps_log;
	std::vector<float> ms_log;
};
