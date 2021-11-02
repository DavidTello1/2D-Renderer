#pragma once
#include "Module.h"

#define FPS_LOG_SIZE 100
#define PANEL_WIDTH 185
#define PANEL_HEIGHT 185

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

	float move_speed = 0.0f;
	float zoom_speed = 0.0f;

	std::vector<float> fps_log;
	std::vector<float> ms_log;
};
