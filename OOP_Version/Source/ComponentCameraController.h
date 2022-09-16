#pragma once
#include "Component.h"
#include "ComponentCamera.h"

#define DEFAULT_MOVE_SPEED 200
#define DEFAULT_ZOOM_SPEED 0.25

class ComponentCameraController : public Component
{
public:
	ComponentCameraController(Entity* entity);
	~ComponentCameraController() {};

	static inline Component::Type GetType() { return Component::Type::CAMERA_CONTROLLER; }

	ComponentCamera* GetCamera() { return camera; }
	const ComponentCamera* GetCamera() const { return camera; }

	const float& GetZoom() const { return zoom; }
	const float& GetZoomSpeed() const { return zoom_speed; }
	const float& GetMoveSpeed() const { return move_speed; }

	void SetZoomSpeed(float speed) { zoom_speed = speed; }
	void SetMoveSpeed(float speed) { move_speed = speed; }

	void OnUpdate(float dt) override;

	void OnResize(int width, int height);
	void OnZoom(int zoom);

private:
	float zoom = 1.0f;

	float move_speed = 0.0f;
	float zoom_speed = 0.0f;

	ComponentCamera* camera = nullptr;
};

