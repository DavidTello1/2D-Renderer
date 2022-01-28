#pragma once
#include "SystemBase.h"

class S_Renderer : public SystemBase
{
public:
    void Init();
    void Render();
};

class S_Physics : public SystemBase
{
public:
    void Init();
    void Update(float dt);
};

class S_CameraController : public SystemBase
{
public:
    void Init();
    void Update(float dt);

    void OnResize(int width, int height);
    void OnZoom(int new_zoom);
};
