#pragma once
#include "Module.h"

#include "PCG/pcg_basic.h"
#include "glm/include/glm/glm.hpp"

class ModuleSceneBase : public Module
{
public:
	ModuleSceneBase(bool start_enabled = true);
	virtual ~ModuleSceneBase();

	void OnWindowResize();
	void OnMouseZoom(int mouse_wheel);

	//// --- Getters
	//ComponentCameraController* GetMainCamera() { return main_camera; }
	const glm::mat4& GetViewProjMatrix() const;
	pcg32_random_t& GetRNG() { return rng; }

	//// --- Setters
	//void SetMainCamera(ComponentCameraController* camera) { main_camera = camera; }

public:
	bool is_draw_grid = false;
	bool is_draw_colliders = false;

private:
	//ComponentCameraController* main_camera = nullptr;

	pcg32_random_t rng;
};

