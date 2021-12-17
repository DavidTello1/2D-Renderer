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

	// --- Getters
	const int& GetGridSize() const { return grid_size; }
	ComponentCameraController* GetMainCamera() { return main_camera; }
	const glm::mat4& GetViewProjMatrix() const;
	pcg32_random_t& GetRNG() { return rng; }

	// --- Setters
	void SetGridSize(int size);
	void SetMainCamera(ComponentCameraController* camera) { main_camera = camera; }

private:
	void DrawAxis();
	void DrawGrid(int size);

public:
	bool is_draw_grid = false;
	bool is_draw_axis = false;
	bool is_draw_colliders = false;

private:
	ComponentCameraController* main_camera = nullptr;

	pcg32_random_t rng;
	int grid_size = 50;
};

