#pragma once
#include "Module.h"

#include "PCG/pcg_basic.h"
#include "glm/include/glm/glm.hpp"

class ComponentCameraController;

class ModuleSceneBase : public Module
{
public:
	ModuleSceneBase(bool start_enabled = true);
	virtual ~ModuleSceneBase();

	void Draw();
	void OnWindowResize();
	void OnMouseZoom(int mouse_wheel);

	const bool& IsDebug() const { return is_debug; }
	void SwitchDebug() { is_debug = !is_debug; }

	// --- Getters
	const int& GetGridSize() const { return grid_size; }
	ComponentCameraController* GetMainCamera() { return main_camera; }
	const glm::mat4& GetViewProjMatrix() const;
	pcg32_random_t& GetRNG() { return rng; }

	// --- Setters
	void SetGridSize(int size) { grid_size = size; }
	void SetMainCamera(ComponentCameraController* camera) { main_camera = camera; }

private:
	void DrawAxis();
	void DrawGrid(int size);

private:
	ComponentCameraController* main_camera = nullptr;

	pcg32_random_t rng;
	bool is_debug = false;
	int grid_size = 5;
};

