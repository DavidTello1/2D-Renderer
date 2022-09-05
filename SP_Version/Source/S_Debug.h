#pragma once
#include "System.h"

#define DEFAULT_GRID_SPACING 100

typedef unsigned int uint;

struct C_Transform;
struct C_Collider;

class S_Debug : public System
{
public:
	S_Debug();
	~S_Debug();

	void Start() override;
	void Update(float dt) override;

	// Render
    void RenderGrid();
    void RenderColliders();

private:
	// Colliders
	uint circle_texture = 0;

	// Grid
	uint line_shader = 0;

	//---
	std::vector<C_Transform> transforms;
	std::vector<C_Collider> colliders;
};
