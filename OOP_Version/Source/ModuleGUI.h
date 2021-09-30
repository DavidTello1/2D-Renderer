#pragma once
#include "Module.h"

#define PANEL_WIDTH 185
#define PANEL_HEIGHT 185
#define YELLOW { 1.00f, 1.00f, 0.00f , 1.00f}

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

public:
	bool is_update_pos = false;
	bool is_debug = false;

private:
	int num_asteroids = 0;
};
