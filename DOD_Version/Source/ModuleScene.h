#pragma once
#include "Module.h"
#include "SceneManager.h"

#include "PCG/pcg_basic.h"
#include <memory>

class S_Renderer;
class S_CameraController;
class S_Physics;

class ModuleScene : public Module
{
public:
	ModuleScene(bool start_enabled = true);
	virtual ~ModuleScene();

	bool Init() override;
	bool Start() override;
	bool Update(float dt) override;
	bool CleanUp() override;

	void Draw();
	pcg32_random_t& GetRNG() { return rng; }

	//*** UNTIL EVENT MANAGER
	void OnResize(int width, int height);
	void OnZoom(int new_zoom);

public:
	Coordinator coordinator;

private:
	std::shared_ptr<S_Renderer> render_system;
	std::shared_ptr<S_CameraController> camera_system;
	std::shared_ptr<S_Physics> physics_system;

	pcg32_random_t rng;
};
