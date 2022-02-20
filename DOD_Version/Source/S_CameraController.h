#pragma once
#include "System.h"

struct EventWindowResize;
struct EventCameraZoom;
struct EventCameraSpeedChanged;

class S_CameraController : public System
{
public:
    void Init();
    void Update(float dt);

private:
    // Events
    void OnResize(EventWindowResize* e);
    void OnZoom(EventCameraZoom* e);
    void OnSpeedChange(EventCameraSpeedChanged* e);
};
