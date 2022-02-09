#pragma once
#include "SystemBase.h"
#include "Event.h"

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

    // Events
    void OnResize(EventWindowResize* e);
    void OnZoom(EventCameraZoom* e);
    void OnSpeedChange(EventCameraSpeedChanged* e);
};
