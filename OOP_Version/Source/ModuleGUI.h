#pragma once
#include "Module.h"

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
	bool is_debug = false;
};
