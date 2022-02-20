#pragma once
#include "Module.h"

#include "glm/include/glm/glm.hpp"

#define BASE_ENTITIES 6 //camera + background + boundaries(4)
#define BOUNDARIES_SIZE 100
#define DEFAULT_WORLD_WIDTH 2500
#define DEFAULT_WORLD_HEIGHT 2500

#define DEFAULT_CAMERA_MOVE_SPEED 200.0
#define DEFAULT_CAMERA_ZOOM_SPEED 0.25

#define MAX_ASTEROID_VELOCITY 150
#define MIN_ASTEROID_VELOCITY 50
#define MIN_ASTEROID_ROTATION_SPEED 5
#define MAX_ASTEROID_ROTATION_SPEED 120
#define MAX_ASTEROID_SIZE 100
#define MIN_ASTEROID_SIZE 50

struct EventAsteroidAdded;
struct EventAsteroidRemoved;
struct EventWorldSizeUpdate;

typedef unsigned int Entity;

class ModuleGame : public Module
{
public:
	ModuleGame(bool start_enabled = true);
	virtual ~ModuleGame();

	bool Init() override;
	bool Start() override;
	bool Update(float dt) override;

	// Main Camera
	const Entity& GetMainCamera() const { return main_camera; }
	void SetMainCamera(Entity camera) { main_camera = camera; }
	const glm::mat4& GetViewProjMatrix() const;

	// World
	const int& GetWorldWidth() const { return world_width; }
	const int& GetWorldHeight() const { return world_height; }

	void SetWorldWidth(int width) { world_width = width; UpdateWorldSize(); }
	void SetWorldHeight(int height) { world_height = height; UpdateWorldSize(); }

	void UpdateWorldSize();

	// Asteroids
	void AddAsteroids(int num);
	void DeleteAsteroids(int num);

private:
	// --- EVENTS ---
	void OnWorldSizeUpdate(EventWorldSizeUpdate* e);
	void OnAddAsteroid(EventAsteroidAdded* e);
	void OnRemoveAsteroid(EventAsteroidRemoved* e);

private:
	Entity main_camera = 0;

	int world_width = 0;
	int world_height = 0;

	Entity background = 0;
	Entity b_top = 0;
	Entity b_bottom = 0;
	Entity b_left = 0;
	Entity b_right = 0;	
};
